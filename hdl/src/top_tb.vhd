library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity TopTb is
--  Port ( );
end TopTb;

architecture Blastn of TopTb is

    component Top
        Generic (
            g_RSTCNT: POSITIVE := 5;
            g_SIZE : POSITIVE := 100;
            g_BITS : POSITIVE := 32
        );
        Port (
            clk : in  STD_LOGIC;
            rst : in  STD_LOGIC;
            rx  : in  STD_LOGIC;
            tx  : out STD_LOGIC
        );
    end component;


    component UartTx
        Generic (
            g_CLK_PER_BIT : POSITIVE:= 100000000 / 256000       -- FPGA clock / baud rate
        );
        Port ( 
            i_clk         : in  STD_LOGIC;
            i_enable      : in  STD_LOGIC;
            i_rst         : in  STD_LOGIC;
            i_byte        : in  STD_LOGIC_VECTOR(7 downto 0);   -- Byte to be transmitted 
            o_tx          : out STD_LOGIC;                      -- Individual serial bits transmitted       
            o_ready       : out STD_LOGIC;                      -- Transmission ready flag
            o_done        : out STD_LOGIC
        );
    end component;

    component UartRx
        Generic ( 
            g_CLK_PER_BIT : POSITIVE:= 100000000 / 256000       -- FPGA clock / baud rate
        );
        Port ( 
            i_clk         : in  STD_LOGIC;
            i_rx          : in  STD_LOGIC;                      -- Serial bit being received/sampled
            i_rst         : in  STD_LOGIC;
            o_byte        : out STD_LOGIC_VECTOR(7 downto 0);
            o_done        : out STD_LOGIC                       -- Byte received flag
        );
    end component;

    signal wt        : TIME:= 5ns;
    
    signal tb_clk     : STD_LOGIC:= '0';
    signal tb_rst     : STD_LOGIC:= '0';
    signal tb_enable  : STD_LOGIC:= '0';
    signal tb_ready   : STD_LOGIC:= '0';
    signal tb_tx      : STD_LOGIC:= '1';
    signal tb_rx      : STD_LOGIC:= '1';
    signal tb_done    : STD_LOGIC:= '0';
    signal tb_tx_done : STD_LOGIC:= '0';

    signal tb_tx_byte : STD_LOGIC_VECTOR(7 downto 0):= (others => '0');
    signal tb_rx_byte : STD_LOGIC_VECTOR(7 downto 0):= (others => '0');

begin

    TOP_INST: Top
    Port map (
        clk      => tb_clk,
        rst      => tb_rst,
        rx       => tb_rx,
        tx       => tb_tx
    );

    UART_TX_INST: UartTx
    Port map (
        i_clk         => tb_clk,
        i_enable      => tb_enable,
        i_rst         => tb_rst,
        i_byte        => tb_tx_byte,
        o_tx          => tb_rx,
        o_ready       => tb_ready,
        o_done        => tb_tx_done
    );
    
    UART_RX_INST: UartRx
    Port map (
        i_clk         => tb_clk,
        i_rx          => tb_tx,
        i_rst         => tb_rst,
        o_byte        => tb_rx_byte,
        o_done        => tb_done
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

        tb_rst     <= '1';
        wait for 4*wt;
        tb_rst     <= '0';
        wait for 4*wt;

        tb_tx_byte <= "00001000";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;               -- Approx. (39062.5 / 5) nano-seconds

        tb_tx_byte <= "00000000";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000000";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000000";       -- Size of query and subject = 8 in this case (0x00000008)
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000010";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000000";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000000";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000000";       -- Gap index = 2 in this case (0x00000002)
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000001";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000000";
        tb_enable  <= '1';
        wait for 2*wt;
        tb_enable  <= '0';
        wait for 7814*wt;

        tb_tx_byte <= "00000000";
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
        wait for 7914*wt;

        wait for 32102*wt;             -- Approx. (((FPGA clock period) * (g_CLK_PER_BIT) * 10) * 4)/5

    end process SIM_GEN;
end Blastn;