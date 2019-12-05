library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity ScoreMatrix is
    generic (
        g_MAXSTEP : POSITIVE := 10;
        g_MATSIZE : POSITIVE := 10; -- square matrix size
        g_BITS : POSITIVE := 32;    -- result size in bits (ie. 32-bit integer result)
        g_LENGTH : POSITIVE := 100  -- length of the query and subject in letters
    );
    port (
        clk         : in  STD_LOGIC;    -- board clock
        rst         : in  STD_LOGIC;    -- reset the score counter
        done        : out  STD_LOGIC;
        rx_done     : in  STD_LOGIC;
        tx_done     : in  STD_LOGIC;
        
        -- Smith-Waterman match, mismatch, and gap scores
        i_match     : in  SIGNED(1 downto 0);
        i_mismatch  : in  SIGNED(1 downto 0);
        i_gap       : in  SIGNED(1 downto 0);
        
        i_length    : in  UNSIGNED(g_BITS - 1 downto 0);            -- length of the query or the subject
        i_query     : in  STD_LOGIC_VECTOR(g_LENGTH * 3 - 1 downto 0);  -- the query, groups of 3 adjacent bits per character
        i_subject   : in  STD_LOGIC_VECTOR(g_LENGTH * 2 - 1 downto 0);  -- the subject, groups of 2 adjacent bits per character
        o_score     : out UNSIGNED(g_BITS - 1 downto 0)             -- Smith-Waterman max score
    );
end ScoreMatrix;

architecture Blastn of ScoreMatrix is

    component Cell
        Port (
            s        : in  STD_LOGIC_VECTOR(1 downto 0);
            q        : in  STD_LOGIC_VECTOR(2 downto 0);
            match    : in  SIGNED(1 downto 0);
            mismatch : in  SIGNED(1 downto 0);
            gap      : in  SIGNED(1 downto 0);
            diag     : in  STD_LOGIC_VECTOR(1 downto 0);
            up       : in  STD_LOGIC_VECTOR(1 downto 0);
            left     : in  STD_LOGIC_VECTOR(1 downto 0);
            score    : out STD_LOGIC_VECTOR(1 downto 0)
        );
    end component;
    
    -- twice as many columns to hold each score (2 adjacent bits)
    type MATRIX is array(0 to g_MATSIZE) of STD_LOGIC_VECTOR(g_MATSIZE * 2 + 1 downto 0);
    signal m_score_matrix : MATRIX := (others => (others => '0'));

    -- Smith-Waterman score counter
    -- refer to page 4 of https://arxiv.org/pdf/1803.02657.pdf

    signal r_score : UNSIGNED(g_BITS - 1 downto 0) := (others => '0');
    signal r_score_en : STD_LOGIC;
    signal rx_rst, tx_rst : STD_LOGIC;
    -- indicate whether any bits at the bottom are 1
    signal r_botbit : STD_LOGIC;

    -- hold the current state of the score system
    type STATE_TYPE is (s0, s1, s2, s3, s4);
    signal r_state, r_next_state : STATE_TYPE;

    -- the next SIZE letters from the query and subject
    signal q_buf : STD_LOGIC_VECTOR(g_MATSIZE * 3 - 1 downto 0);
    signal s_buf : STD_LOGIC_VECTOR(g_MATSIZE * 2 - 1 downto 0);

    -- auto disable max value for the current score segment
    signal r_step_count : UNSIGNED(g_BITS - 1 downto 0);
    signal r_step_rst : STD_LOGIC;

    -- Count number of letters processed. Indicate scoring is over when
    -- this value equals the query_size
    signal r_shift_count : UNSIGNED(g_BITS - 1 downto 0);

begin

    r_botbit <= '1' when UNSIGNED(m_score_matrix(g_MATSIZE - 1)(g_MATSIZE * 2 - 1 downto 0)) > 0 else '0';
    o_score  <= r_score;

    SCORE_COUNTER: process (clk)
    begin
        if rising_edge(clk) then
            if rst = '1' then
                r_score <= (others => '0');
            elsif r_score_en = '1' then
                r_score <= r_score + 1;
            else
                r_score <= r_score;
            end if;
        end if;
    end process;

    STEP_COUNTER: process (clk)
    begin
        if rising_edge(clk) then
            if r_step_rst = '1' then
                r_step_count <= (others => '0');
            else
                r_step_count <= r_step_count + 1;
            end if;
        end if;
    end process;

    NEXT_STATE: process(clk, rst)
    begin
        if rising_edge(clk) then
            if rst = '1' then
                r_state <= s0;
            else
                r_state <= r_next_state;
            end if;
        end if;
    end process;

    STATE_MACHINE: process(clk, r_state, i_query, i_subject)
    begin
        case r_state is
            -- start
            when s0 =>
                r_step_rst <= '1';
                r_score_en <= '0';
                q_buf <= (others => '0');
                s_buf <= (others => '0');
                done  <= '0';
                r_shift_count <= (others => '0');
                
                if rx_done = '1' then
                    r_next_state <= s1;
                else
                    r_next_state <= s0;
                end if;
            -- load
            when s1 => 
                r_score_en <= '1';
                r_step_rst <= '1';

                q_buf <=   i_query((to_integer(r_shift_count) + g_MATSIZE) * 3 - 1 downto to_integer(r_shift_count) * 3);
                s_buf <= i_subject((to_integer(r_shift_count) + g_MATSIZE) * 2 - 1 downto to_integer(r_shift_count) * 2);
                r_next_state <= s2;
            when s2 => 
                r_step_rst <= '0';
                r_shift_count <= r_shift_count + g_MATSIZE;
                r_next_state <= s3;
            when s3 => 
                if r_botbit = '1' or r_step_count >= g_MAXSTEP then
                    if r_shift_count >= i_length then
                        r_next_state <= s4;
                    else
                        r_next_state <= s1;
                    end if;
                else
                    r_next_state <= s3;
                end if;
            when s4 => 
                r_score_en <= '0';
                done <= '1';
                
                if tx_done = '1' then
                    r_next_state <= s0;
                else
                    r_next_state <= s4;
                end if;
            when others => r_next_state <= s0;
        end case;
    end process;

    -- traverse each query and subject letter
    ROW: for i in 1 to g_MATSIZE generate
        COLUMN: for j in 1 to g_MATSIZE generate
            CURRENT_CELL: Cell
                port map (
                    -- 0 base index
                    s        => s_buf(i*2 - 1 downto i*2 - 2), -- 1,0; 3,2; 5,4 ...
                    q        => q_buf(j*3 - 1 downto j*3 - 3), -- 2,1,0; 5,4,3 ...
                    match    => i_MATCH,
                    mismatch => i_MISMATCH,
                    gap      => i_GAP,
                    -- y axis: 1 base index, x axis: 2 base index
                    diag     => m_score_matrix(i - 1)(j*2 - 1 downto j*2 - 2),
                    up       => m_score_matrix(i - 1)(j*2 + 1 downto j*2    ),
                    left     => m_score_matrix(i    )(j*2 - 1 downto j*2 - 2),
                    score    => m_score_matrix(i    )(j*2 + 1 downto j*2    )
                );
        end generate;
    end generate;

end Blastn;
