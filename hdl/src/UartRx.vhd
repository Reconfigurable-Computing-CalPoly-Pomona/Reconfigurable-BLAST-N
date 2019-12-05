library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity UartRx is
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
end UartRx;

architecture Blastn of UartRx is

    type STATE_MACHINE is (s_idle, s_start_bit, s_receive_bits, s_stop_bit, s_clean_up);

    -- System Initialization
    signal state      : STATE_MACHINE:= s_idle;

    signal temp_done  : STD_LOGIC:= '0';
    signal temp_byte  : STD_LOGIC_VECTOR(7 downto 0):= (others => '0');

    signal count      : NATURAL range 0 to g_CLK_PER_BIT - 1:= 0;
    signal index      : NATURAL range 0 to 7:= 0;

begin

    STATE_CONTROL: process (i_clk) is
    begin
        if (rising_edge(i_clk)) then
            if (i_rst = '1') then
                state     <= s_idle;
                temp_done <= '0';
                temp_byte <= (others => '0');
            else
                case state is
                    when s_idle =>
                        temp_done <= '0';
                        index <= 0;
                        count <= 0;

                        if (i_rx = '0') then
                            state <= s_start_bit;
                        end if;

                    when s_start_bit =>
                        if (count < (g_CLK_PER_BIT - 1) / 2) then   -- Waiting for middle of bit
                            count <= count + 1;
                        else
                            count <= 0;

                            if (i_rx = '0') then                    -- Ensure start bit still low
                                state <= s_receive_bits;
                            else
                                state <= s_idle;
                            end if;
                        end if;

                    when s_receive_bits =>
                        if (count < g_CLK_PER_BIT - 1) then
                            count <= count + 1;
                        else
                            count <= 0;
                            temp_byte(index) <= i_rx;               -- Gathers transmitted byte, starting from LSB

                            if index < 7 then
                                index <= index + 1;
                            else
                                state <= s_stop_bit;
                            end if;
                        end if;

                    when s_stop_bit =>
                        if (count < g_CLK_PER_BIT - 1) then
                            count <= count + 1;
                        else
                            state <= s_clean_up;
                        end if;

                    when s_clean_up =>
                        temp_done <= '1';                       -- Byte transfer is complete
                        index <= 0;
                        count <= 0;
                        state <= s_idle;

                    when others =>
                        state <= s_idle;

                end case;
            end if;
        end if;
    end process STATE_CONTROL;

    o_done <= temp_done;
    o_byte <= temp_byte;

end Blastn;