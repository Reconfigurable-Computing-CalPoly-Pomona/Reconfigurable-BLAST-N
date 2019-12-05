library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity Collector is
    Generic ( 
        g_CLK_PER_BIT : POSITIVE:= 100000000 / 256000;          -- FPGA clock / baud rate
        g_SIZE        : POSITIVE:= 10;      -- Max. size of registers
        g_BITS        : POSITIVE:= 32
    );
    Port ( 
        i_clk         : in  STD_LOGIC;
        i_rx          : in  STD_LOGIC;
        i_rst         : in  STD_LOGIC;
        o_query       : out STD_LOGIC_VECTOR(g_SIZE * 3 - 1 downto 0);
        o_subject     : out STD_LOGIC_VECTOR(g_SIZE * 2 - 1 downto 0);
        co_done       : out STD_LOGIC;
        p_size        : out POSITIVE;
        o_index       : out NATURAL;
        o_count       : out NATURAL;

        -- *** Used for thorough simulation of Collector
        --o_size        : out STD_LOGIC_VECTOR(g_BITS - 1 downto 0);
        --o_gap_start   : out STD_LOGIC_VECTOR(g_BITS - 1 downto 0);
        --o_gap_count   : out STD_LOGIC_VECTOR(g_BITS - 1 downto 0);
        co_byte       : out STD_LOGIC_VECTOR(7 downto 0)
    );
end Collector;

architecture Blastn of Collector is

    component UartRx
        Generic ( 
            g_CLK_PER_BIT : POSITIVE:= 100000000 / 256000
        );
        Port ( 
            i_clk         : in  STD_LOGIC;
            i_rx          : in  STD_LOGIC;                      -- Serial bit transmitted to rx
            i_rst         : in  STD_LOGIC;
            o_byte        : out STD_LOGIC_VECTOR(7 downto 0);
            o_done        : out STD_LOGIC                       -- Byte received flag
        );
    end component;

    type STATE_MACHINE is (s_size, s_gap_start, s_gap_count, s_query, s_subject);
    signal state         : STATE_MACHINE:= s_size;

    signal int_size      : POSITIVE:= 1;
    signal index         : NATURAL:= 0;
    signal int_gap_start : NATURAL:= 0;
    signal int_gap_count : NATURAL:= 0;
    
    signal done          : STD_LOGIC:= '0';                     -- Used as UartRx's Done Flag
    signal temp_done     : STD_LOGIC:= '0';                     -- Used as Collector's Done Flag
    
    signal temp          : STD_LOGIC_VECTOR(7 downto 0):=  (others => '0');     -- Byte received by uart
    
    signal r_size        : STD_LOGIC_VECTOR(g_BITS - 1 downto 0):= (others => '0');
    signal gap_start     : STD_LOGIC_VECTOR(g_BITS - 1 downto 0):= (others => '0');
    signal gap_count     : STD_LOGIC_VECTOR(g_BITS - 1 downto 0):= (others => '0');
    
    signal query         : STD_LOGIC_VECTOR(g_SIZE * 3 - 1 downto 0):= (others => '0');
    signal subject       : STD_LOGIC_VECTOR(g_SIZE * 2 - 1 downto 0):= (others => '0');

begin

    BYTE_RX: UartRx 
        Generic map (
            g_CLK_PER_BIT => g_CLK_PER_BIT
        )
        Port map ( 
            i_clk         => i_clk,
            i_rx          => i_rx,
            i_rst         => i_rst,
            o_byte        => temp,
            o_done        => done
        );

    STATE_CONTROL: process (i_clk) is
    begin          
        if rising_edge(i_clk) then
            if (i_rst = '1') then
                state     <= s_size;
                temp_done <= '0';
                index     <= 0;
                r_size    <= (others => '0');
                gap_start <= (others => '0');
                gap_count <= (others => '0');
                query     <= (others => '0');
                subject   <= (others => '0');
            else
                case state is
                    when s_size =>
                        temp_done <= '0';

                        if (done = '1') then
                            r_size(index + 7 downto index) <= temp;
                            
                            if (index + 8 > g_BITS - 8) then
                                index    <= 0;
                                int_size <= to_integer(UNSIGNED(r_size));
                                state    <= s_gap_start;
                            else
                                index    <= index + 8;
                            end if;
                        end if;
                   
                    when s_gap_start =>
                        if (done = '1') then
                            gap_start(index + 7 downto index) <= temp;
                            
                            if (index + 8 > g_BITS - 8) then
                                index         <= 0;
                                int_gap_start <= to_integer(UNSIGNED(gap_start));
                                state         <= s_gap_count;
                            else
                                index         <= index + 8;
                            end if;
                        end if;
                    
                    when s_gap_count =>
                        if (done = '1') then
                            gap_count(index + 7 downto index) <= temp;
                            
                            if (index + 8 > g_BITS - 8) then
                                index         <= 0;
                                int_gap_count <= to_integer(UNSIGNED(gap_count));
                                state         <= s_query;
                            else
                                index         <= index + 8;
                            end if;
                        end if;
                    
                    -- *** Can never send less than a byte; on PC side, need to append 0 if sending only 3 char for example (4 char = byte)
                    -- *** in byte packing method maybe?
                    
                    when s_query =>
                        if (done = '1') then
                            query(index + 7 downto index) <= temp;
                            
                            if (index + 8 > (int_size * 2) - 8) then
                                index <= 0;
                                state <= s_subject;
                            else
                                index <= index + 8;
                            end if;
                        end if;

                    when s_subject =>
                        if (done = '1') then
                            subject(index + 7 downto index) <= temp;

                            if (index + 8 > (int_size * 2) - 8) then
                                temp_done <= '1';
                                index     <= 0;
                                state     <= s_size;
                            else
                                index     <= index + 8;
                            end if;
                        end if;

                     when others => 
                        state <= s_size;
                end case;
            end if;
        end if;
    end process STATE_CONTROL;

    o_subject   <= subject;
    o_query     <= query;
    co_done     <= temp_done;
    p_size      <= int_size;
    o_index     <= int_gap_start;
    o_count     <= int_gap_count;

    -- *** Used for thorough simulation of Collector
    --o_size      <= r_size;
    --o_gap_start <= gap_start;
    --o_gap_count <= gap_count;
    co_byte     <= temp;

end Blastn;