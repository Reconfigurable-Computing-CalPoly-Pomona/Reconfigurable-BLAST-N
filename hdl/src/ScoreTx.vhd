library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity ScoreTx is
    Generic (
        g_CLK_PER_BIT : POSITIVE:= 100000000 / 256000;          -- FPGA clock / baud rate
        g_BITS        : POSITIVE:= 32
    );
    Port (
        i_clk         : in  STD_LOGIC;
        i_enable      : in  STD_LOGIC;
        i_rst         : in  STD_LOGIC;
        i_score       : in  STD_LOGIC_VECTOR(g_BITS - 1 downto 0);
        o_tx          : out STD_LOGIC;
        o_ready       : out STD_LOGIC
    );
end ScoreTx;

architecture Blastn of ScoreTx is

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
    
    type STATE_MACHINE is (s_idle, s_byte0, s_pre_byte1, s_byte1, s_pre_byte2, s_byte2, s_pre_byte3, s_byte3);
    signal state      : STATE_MACHINE:= s_idle;
    
    signal tx_enable  : STD_LOGIC:= '0';
    signal tx_ready   : STD_LOGIC:= '0';
    signal tx_done    : STD_LOGIC:= '0';
    signal temp_ready : STD_LOGIC:= '0';

    signal temp_byte  : STD_LOGIC_VECTOR(7 downto 0):= (others => '0');
    
begin

    BYTE_TX: UartTx
        Generic map (
            g_CLK_PER_BIT => g_CLK_PER_BIT
        )
        Port map (
            i_clk         => i_clk,
            i_enable      => tx_enable,
            i_rst         => i_rst,
            i_byte        => temp_byte,
            o_tx          => o_tx,
            o_ready       => tx_ready,
            o_done        => tx_done
        );
    
    STATE_CONTROL: process (i_clk) is
    begin

        if rising_edge(i_clk) then
            if (i_rst = '1') then
                state      <= s_idle;
                tx_enable  <= '0';
                temp_ready <= '0';
                temp_byte  <= (others => '0');
            else
                case state is
                    when s_idle =>
                        tx_enable      <= '0';
                        temp_byte      <= i_score(7 downto 0);

                        if (tx_ready = '1') then
                            temp_ready <= '1';
                            
                            if (i_enable = '1') then
                                state  <= s_byte0;
                            end if;
                        else
                            temp_ready <= '0';
                        end if;

                    when s_byte0 =>
                        tx_enable  <= '1';
                        temp_ready <= '0';
                        state      <= s_pre_byte1;

                    when s_pre_byte1 =>
                        tx_enable  <= '0';
                        temp_byte <= i_score(15 downto 8);
                        
                        if (tx_done = '1') then
                            state <= s_byte1;
                        end if;

                    when s_byte1 =>
                        if (tx_ready = '1') then
                            tx_enable  <= '1';
                            state      <= s_idle;
                            state      <= s_pre_byte2;
                        end if;

                    when s_pre_byte2 =>
                        tx_enable  <= '0';
                        temp_byte <= i_score(23 downto 16);
                        
                        if (tx_done = '1') then
                            state <= s_byte2;
                        end if;

                    when s_byte2 =>
                        if (tx_ready = '1') then
                            tx_enable  <= '1';
                            state      <= s_pre_byte3;
                        end if;

                    when s_pre_byte3 =>
                        tx_enable  <= '0';
                        temp_byte <= i_score(31 downto 24);
                        
                        if (tx_done = '1') then
                            state <= s_byte3;
                        end if;

                    when s_byte3 =>
                        if (tx_ready = '1') then
                            tx_enable  <= '1';
                            state      <= s_idle;
                        end if;

                    when others =>
                        state <= s_idle;

                end case;
            end if;
        end if;
    end process STATE_CONTROL;

    o_ready  <= temp_ready;

end Blastn;