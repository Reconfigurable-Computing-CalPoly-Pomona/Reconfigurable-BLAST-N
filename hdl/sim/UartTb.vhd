library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity UartTb is
    Generic (
        tb_CLK_PER_BIT : POSITIVE:= 100000000 / 256000          -- Needs to be set correctly
    );
end UartTb;

architecture Blastn of UartTb is

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
 
    component UartRx
        Generic ( 
            g_CLK_PER_BIT : POSITIVE:= 100000000 / 256000       -- FPGA clock / baud rate
        );
        Port ( 
            i_clk         : in  STD_LOGIC;
            i_rx          : in  STD_LOGIC;                      -- Serial bit transmitted to rx
            o_byte        : out STD_LOGIC_VECTOR(7 downto 0);
            o_done        : out STD_LOGIC                       -- Byte received flag
        );
    end component;
    
    constant wt       : TIME:= 5ns;              -- (FPGA clock period)/2
  
    signal tb_clk     : STD_LOGIC:= '0';
    signal tb_enable  : STD_LOGIC:= '0';
    signal tb_ready   : STD_LOGIC:= '0';
    signal tb_tx      : STD_LOGIC:= '1';
    signal tb_done    : STD_LOGIC:= '0';
    
    signal tb_tx_byte : STD_LOGIC_VECTOR(7 downto 0):= (others => '0');     -- Byte to be transmitted
    signal tb_rx_byte : STD_LOGIC_VECTOR(7 downto 0):= (others => '0');     -- Byte to be received
   
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
 
    UART_RX_INST: UartRx 
        Generic map (
            g_CLK_PER_BIT => tb_CLK_PER_BIT
        )
        Port map (
            i_clk         => tb_clk,
            i_rx          => tb_tx,         -- Output of transmitter = input of receiver
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
    
        tb_tx_byte <= "11000011";
        tb_enable  <= '1';
        wait for wt;
        tb_enable  <= '0';
        wait for 7813*wt;        -- Approx. (39062.5 / 5) nano-seconds
        
        tb_tx_byte <= "00000001";
        tb_enable  <= '1';
        wait for wt;
        tb_enable  <= '0';
        wait for 7813*wt;
    
    end process SIM_GEN;
end Blastn;