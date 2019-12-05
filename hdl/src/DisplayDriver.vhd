--library IEEE;
--use IEEE.STD_LOGIC_1164.ALL;

--entity DisplayDriver is
--    Generic ( 
--        g_COUNT_SIZE  : INTEGER:= 32;
--        g_CLK_PER_BIT : POSITIVE:= 100000000 / 256000;          -- FPGA clock / baud rate
--        g_SIZE        : POSITIVE:= 16;      -- Max. size of registers
--        g_BITS        : POSITIVE:= 32
--    );
--    Port ( 
--        i_clk         : in  STD_LOGIC;
--        i_rst         : in  STD_LOGIC;
--        i_rx          : in  STD_LOGIC;
--        o_done        : out STD_LOGIC;
--        dp            : out STD_LOGIC;
--        dig_c         : out STD_LOGIC_VECTOR (6 downto 0);
--        an            : out STD_LOGIC_VECTOR (7 downto 0)
--    );
--end DisplayDriver;

--architecture Blastn of DisplayDriver is

--    component CounterGeneric
--        Generic (
--            g_COUNT_SIZE : integer := 32
--        );
--        Port (
--            clk          : in  STD_LOGIC;
--            rst          : in  STD_LOGIC;
--            count        : out STD_LOGIC_VECTOR (g_COUNT_SIZE - 1 downto 0)
--        );
--    end component;

--    component SegmentEncoder
--        Port (  
--            S          : in  STD_LOGIC_VECTOR (3 downto 0);
--            dig        : out STD_LOGIC_VECTOR (6 downto 0)
--        );
--    end component;

--    component Collector
--        Generic ( 
--            g_CLK_PER_BIT : POSITIVE:= 100000000 / 256000;          -- FPGA clock / baud rate
--            g_SIZE        : POSITIVE:= 16;      -- Max. size of registers
--            g_BITS        : POSITIVE:= 32
--        );
--        Port ( 
--            i_clk         : in  STD_LOGIC;
--            i_rx          : in  STD_LOGIC;
--            o_query       : out STD_LOGIC_VECTOR(g_SIZE - 1 downto 0);
--            o_subject     : out STD_LOGIC_VECTOR(g_SIZE - 1 downto 0);
--            co_done       : out STD_LOGIC
--            --o_size        : out POSITIVE;
--            --o_index       : out NATURAL;
--            --o_count       : out NATURAL
--        );
--    end component;
    
--    signal done           : STD_LOGIC:= '0';
--    signal temp_done      : STD_LOGIC:= '0';
    
--    signal count_out      : STD_LOGIC_VECTOR (g_COUNT_SIZE - 1 downto 0):= (others => '0');
--    signal dig_choice     : STD_LOGIC_VECTOR (3 downto 0):= (others => '0');
--    signal subject        : STD_LOGIC_VECTOR (g_SIZE - 1 downto 0):= (others => '0');
--    signal query          : STD_LOGIC_VECTOR(g_SIZE - 1 downto 0):= (others => '0');

--begin

--    COUNT_INST: CounterGeneric
--        Generic map ( 
--            g_COUNT_SIZE  => g_COUNT_SIZE
--        )
--        Port map (
--            clk           => i_clk,
--            rst           => i_rst,
--            count         => count_out
--        );

--    SEG_ENC_INST: SegmentEncoder
--        Port map (
--            S             => dig_choice,
--            dig           => dig_c
--        );

--    COLLECT_INST: Collector
--        Generic map (
--            g_CLK_PER_BIT => g_CLK_PER_BIT,
--            g_SIZE        => g_SIZE,
--            g_BITS        => g_BITS
--        )
--        Port map (
--            i_clk         => i_clk,
--            i_rx          => i_rx,
--            o_query       => query,
--            o_subject     => subject,
--            co_done       => done
--        );

--    dp <= '1';
    
--    DONE_CONTROL: process (i_clk) is
--    begin

--        if (done = '1') then
--            o_done <= not temp_done;
--        end if;

--    end process DONE_CONTROL;
    
--    -- Turning on each display, using counter bits as a clock  
--    AN_CONTROL: process (count_out(g_COUNT_SIZE - 12 downto g_COUNT_SIZE - 14)) is
--    begin              
--        case count_out(g_COUNT_SIZE - 12 downto g_COUNT_SIZE - 14) is
--            when "000"  => an <= "11111110";               
--            when "001"  => an <= "11111101";                       
--            when "010"  => an <= "11111011";                 
--            when "011"  => an <= "11110111";
--            when "100"  => an <= "11101111";
--            when "101"  => an <= "11011111";                                    
--            when "110"  => an <= "10111111";             
--            when "111"  => an <= "01111111";
--            when others => an <= (others => 'Z');                 -- all other displays turned off 
--        end case;               
--    end process AN_CONTROL;                  
    
--    -- Controlling what the data for each display, in sync with the previous process using counter bits         
--    DIG_CONT: process (count_out(g_COUNT_SIZE - 12 downto g_COUNT_SIZE - 14)) is
--    begin      
--        case count_out(g_COUNT_SIZE - 12 downto g_COUNT_SIZE - 14) is
--            when "000"  => dig_choice <= query(3 downto 0);
--            when "001"  => dig_choice <= query(7 downto 4);
--            when "010"  => dig_choice <= query(11 downto 8);
--            when "011"  => dig_choice <= query(15 downto 12);
--            when "100"  => dig_choice <= subject(3 downto 0);  
--            when "101"  => dig_choice <= subject(7 downto 4);  
--            when "110"  => dig_choice <= subject(11 downto 8); 
--            when "111"  => dig_choice <= subject(15 downto 12);
--            when others => dig_choice <= (others => 'Z'); 
--        end case;            
--    end process DIG_CONT;
--end Blastn;