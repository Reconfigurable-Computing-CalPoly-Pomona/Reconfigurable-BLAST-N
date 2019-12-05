library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity CollectorTb is
    Generic (
        tb_CLK_PER_BIT : POSITIVE:= 100000000 / 256000;         -- Needs to be set correctly
        tb_SIZE        : POSITIVE:= 32;
        tb_BITS        : POSITIVE:= 10
    );
end CollectorTb;

architecture Blastn of CollectorTb is

    component UartTx
        Generic (
            g_CLK_PER_BIT : POSITIVE:= 100000000 / 256000       -- FPGA clock / baud rate
        );
        Port ( 
            i_clk         : in  STD_LOGIC;
            i_enable      : in  STD_LOGIC;
            i_byte        : in  STD_LOGIC_VECTOR(7 downto 0);   -- Byte to be transmitted 
            o_tx          : out STD_LOGIC;                      -- Individual serial bits transmitted       
            o_ready       : out STD_LOGIC                       -- Transmission ready flag
        );
    end component;

    component Collector
        Generic ( 
            g_CLK_PER_BIT : POSITIVE:= 100000000 / 256000;
            g_SIZE        : POSITIVE:= 10;
            g_BITS        : POSITIVE:= 32
        );
        Port ( 
            i_clk         : in  STD_LOGIC;
            i_rx          : in  STD_LOGIC;
            o_query       : out STD_LOGIC_VECTOR(g_SIZE * 3 - 1 downto 0);
            o_subject     : out STD_LOGIC_VECTOR(g_SIZE * 2 - 1 downto 0);
            co_done       : out STD_LOGIC
            
            -- *** Used for thorough simulation of Collector
            --o_size        : out STD_LOGIC_VECTOR(g_BITS - 1 downto 0);
            --o_gap_start   : out STD_LOGIC_VECTOR(g_BITS - 1 downto 0);
            --o_gap_count   : out STD_LOGIC_VECTOR(g_BITS - 1 downto 0);
            --co_byte       : out STD_LOGIC_VECTOR(7 downto 0)
        );
    end component;

    constant wt       : TIME:= 5ns;         -- (FPGA clock period)/2
  
    signal tb_clk     : STD_LOGIC:= '0';
    signal tb_enable  : STD_LOGIC:= '0';
    signal tb_ready   : STD_LOGIC:= '0';
    signal tb_tx      : STD_LOGIC:= '1';
    signal tb_done    : STD_LOGIC:= '0';
    
    signal tb_tx_byte : STD_LOGIC_VECTOR(7 downto 0):= (others => '0');                 -- Byte to be transmitted
    signal tb_query   : STD_LOGIC_VECTOR(tb_SIZE * 3 - 1 downto 0):= (others => '0');
    signal tb_subject : STD_LOGIC_VECTOR(tb_SIZE * 2 - 1 downto 0):= (others => '0');

    -- *** Used for thorough simulation of Collector
    --signal tb_rx_byte : STD_LOGIC_VECTOR(7 downto 0):= (others => '0');
    --signal tb_o_size  : STD_LOGIC_VECTOR(tb_BITS - 1 downto 0):= (others => '0');
    --signal tb_gap_start : STD_LOGIC_VECTOR(tb_BITS - 1 downto 0):= (others => '0');
    --signal tb_gap_count : STD_LOGIC_VECTOR(tb_BITS - 1 downto 0):= (others => '0');
    --signal tb_done    : STD_LOGIC:= '0';

begin
 
    UART_TX_INST: UartTx
        Generic map (
            g_CLK_PER_BIT => tb_CLK_PER_BIT
        )
        Port map (
            i_clk         => tb_clk,
            i_enable      => tb_enable,
            i_byte        => tb_tx_byte,
            o_tx          => tb_tx,
            o_ready       => tb_ready
        );
 
    COLLECTOR_INST: Collector 
        Generic map (
            g_CLK_PER_BIT => tb_CLK_PER_BIT,
            g_SIZE        => tb_SIZE,
            g_BITS        => tb_BITS
        )
        Port map (
            i_clk         => tb_clk,
            i_rx          => tb_tx,             -- Serial bit output of UartTx
            o_query       => tb_query,
            o_subject     => tb_subject,
            co_done       => tb_done
            --o_size        => tb_o_size,
            --o_gap_start   => tb_gap_start,
            --o_gap_count   => tb_gap_count,
            --co_byte       => tb_rx_byte
        );
        
    CLK_GEN: process      -- Generates 100 MHz clock signal
    begin
                
        tb_clk <= '1';
        wait for wt;
        tb_clk <= '0';
        wait for wt;

    end process CLK_GEN;
                
    SIM_GEN: process
    begin
        
        -- (FPGA clock period) * (g_CLK_PER_BIT) for 1 bit
        -- 1 start bit, 8 bit data, 1 stop bit
        -- Approx. 39.0625 micro-seconds for 1 byte to transfer (100 MHz clock and 256000 baud rate)
    
        tb_tx_byte <= "00001000";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;               -- Approx. (39062.5 / 5) nano-seconds

        tb_tx_byte <= "00000000";       -- Size of query and subject = 8 in this case (0x0008)
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000010";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000000";       -- Gap index = 2 in this case (0x0002)
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000001";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000000";       -- Gap count = 1 in this case
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00001111";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "11110000";       -- Query = 0xF00F
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "11110000";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00001111";       -- Subject = 0x0FF0
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;
        
        wait for 10*wt;
        
        -- Data for Second Simulation Run
        tb_tx_byte <= "00010000";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000000";       -- Size of query and subject = 16 in this case (0x0010)
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00010001";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000000";       -- Gap index = 17 in this case (0x0011)
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000011";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000000";       -- Gap count = 3 in this case
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "11000011";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00010000";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00100100";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "11110000";       -- Query = 0xF02410C3
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00001010";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "10100000";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00010111";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "11111000";       -- Subject = 0xF817A00A
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

    end process SIM_GEN;
end Blastn;