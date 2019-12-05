library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity ScoreMatrixSq is
    generic (
        g_RSTCNT : POSITIVE := 10;
        g_MATSIZE : POSITIVE := 10; -- square matrix size in letters
        g_BITS : POSITIVE := 32     -- result size in bits (ie. 32-bit integer result)
    );
    port (
        clk         : in  STD_LOGIC;    -- board clock
        rst         : in  STD_LOGIC;    -- reset the score counter
        done        : out STD_LOGIC;
        
        -- Smith-Waterman match, mismatch, and gap scores
        i_match     : in  SIGNED(1 downto 0);
        i_mismatch  : in  SIGNED(1 downto 0);
        i_gap       : in  SIGNED(1 downto 0);
        
        i_query     : in  STD_LOGIC_VECTOR(g_MATSIZE * 3 - 1 downto 0);  -- the query, groups of 3 adjacent bits per character
        i_subject   : in  STD_LOGIC_VECTOR(g_MATSIZE * 2 - 1 downto 0);  -- the subject, groups of 2 adjacent bits per character
        o_score     : out UNSIGNED(g_BITS - 1 downto 0)             -- Smith-Waterman max score
    );
end ScoreMatrixSq;

architecture Blastn of ScoreMatrixSq is

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
    signal r_en : STD_LOGIC;
    -- indicate whether any bits at the bottom are 1
    signal r_dis : STD_LOGIC;

    -- the next SIZE letters from the query and subject
    signal q_buf : STD_LOGIC_VECTOR(g_MATSIZE * 3 - 1 downto 0);
    signal s_buf : STD_LOGIC_VECTOR(g_MATSIZE * 2 - 1 downto 0);

    signal rst_counter : UNSIGNED(g_BITS - 1 downto 0);
    signal rst_sig : STD_LOGIC;

begin

    r_dis <= '1' when UNSIGNED(m_score_matrix(g_MATSIZE - 1)(g_MATSIZE * 2 - 1 downto 0)) > 0 else '0';
    o_score  <= r_score;

    SCORE_COUNTER: process (clk)
    begin
        if rising_edge(clk) then
            if rst = '1' then
                r_score <= (others => '0');
                --r_en <= '1';
                done <= '0';
            elsif r_en = '1' and r_dis = '0' then
                r_score <= r_score + 1;
                done <= '0';
            else
                r_score <= r_score;
                done <= '1';
            end if;
        end if;
    end process;

    RESET_PROC: process (clk)
    begin
        if rising_edge(clk) then
            if rst = '1' then
                s_buf <= (others => '0');
                q_buf <= (others => '0');
                rst_sig <= '1';
            else
                s_buf <= i_subject;
                q_buf <= i_query;
                rst_sig <= '0';
            end if;

            if rst_sig = '1' then
                rst_counter <= (others => '0');
            elsif rst_counter >= g_RSTCNT then
                rst_counter <= (others => '0');
                r_en <= '0';
            else
                rst_counter <= rst_counter + 1;
                r_en <= '1';
            end if;
        end if;
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
