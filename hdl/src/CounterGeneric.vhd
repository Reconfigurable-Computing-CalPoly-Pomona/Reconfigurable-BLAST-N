--library IEEE;
--use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.STD_LOGIC_UNSIGNED.ALL;
--use IEEE.STD_LOGIC_ARITH.ALL;

--entity CounterGeneric is
--    Generic (
--        g_COUNT_SIZE : INTEGER:= 32
--    );
--    Port ( 
--        clk        : in  STD_LOGIC;
--        rst        : in  STD_LOGIC;
--        count      : out STD_LOGIC_VECTOR(g_COUNT_SIZE - 1 downto 0)
--    );
--end CounterGeneric;

--architecture Blastn of CounterGeneric is

--    signal temp: std_logic_vector(g_COUNT_SIZE - 1 downto 0);

--begin

--    COUNT_GEN: process (clk, rst) is
--    begin
--        if (rst = '1') then 
--            temp <= (others =>'0');
--        elsif (rising_edge(clk)) then
--            temp <= temp + 1;
--        end if;
--    end process COUNT_GEN;

--    count <= temp;
           
--end Blastn;