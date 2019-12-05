library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity UartTx is
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
end UartTx;

architecture Blastn of UartTx is

    type STATE_MACHINE is (s_idle, s_start_bit, s_transmit_bits, s_stop_bit, s_clean_up);
    
    -- System Initialization
    signal state      : STATE_MACHINE:= s_idle;
    
    signal temp_ready : STD_LOGIC:= '0';
    signal temp_done  : STD_LOGIC:= '0';
    signal temp_byte  : STD_LOGIC_VECTOR(7 downto 0):= (others => '0');
    
    signal count      : NATURAL range 0 to g_CLK_PER_BIT - 1:= 0;
    signal index      : NATURAL range 0 to 7:= 0;

begin
    
    STATE_CONTROL: process (i_clk) is
    begin
        if (rising_edge(i_clk)) then
            if (i_rst = '1') then
                state      <= s_idle;
                o_tx       <= '1';
                temp_ready <= '0';
                temp_done  <= '0';
                temp_byte  <= (others => '0');
            else
                case state is
                    when s_idle =>
                        temp_ready    <= '1';
                        o_tx          <= '1';                 -- Waiting for start bit of 0
                        temp_done     <= '0';
                        index <= 0;
                        count <= 0;
                        
                        if (i_enable = '1') then              -- "i_enable" begins transmission
                            temp_byte <= i_byte;
                            state     <= s_start_bit;
                        end if;
                    
                    when s_start_bit =>
                        temp_ready <= '0';
                        o_tx       <= '0';                -- Start bit = 0

                        if (count < g_CLK_PER_BIT - 1) then   -- Wait until start bit ends
                            count  <= count + 1;
                        else
                            count  <= 0;
                            state  <= s_transmit_bits;
                        end if;
    
                    when s_transmit_bits =>
                        o_tx <= temp_byte(index);             -- Serial bit data taken from input byte, starting from LSB
                        
                        if (count < g_CLK_PER_BIT - 1) then
                            count  <= count + 1;
                        else
                            count  <= 0;
                            
                            if (index < 7) then               -- 8 serial bits sent before moving to stop bit
                                index <= index + 1;
                            else
                                state <= s_stop_bit;
                            end if;
                        end if;
                    
                    when s_stop_bit =>
                        o_tx <= '1';                          -- Stop bit = 1
                        
                        if (count < g_CLK_PER_BIT - 1) then
                            count  <= count + 1;
                        else
                            state  <= s_clean_up;
                        end if;
                        
                    when s_clean_up =>
                        temp_done  <= '1';
                        count      <= 0;
                        index      <= 0;
                        state      <= s_idle;
                    
                    when others =>
                        state <= s_idle;

                end case;
            end if;
        end if;
    end process STATE_CONTROL;
    
    o_done  <= temp_done;
    o_ready <= temp_ready;

end Blastn;