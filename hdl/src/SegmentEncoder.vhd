--library IEEE;
--use IEEE.STD_LOGIC_1164.ALL;

--entity SegmentEncoder is
--    Port ( 
--        S   : in  STD_LOGIC_VECTOR (3 downto 0);
--        dig : out STD_LOGIC_VECTOR (6 downto 0)
--    );
--end SegmentEncoder;

--architecture Blastn of SegmentEncoder is

--begin


--    ENCODER: process (s) is
--    begin
         
--        CASE_ENCODER: case s is
--                               -- "bcdefag" where '0' = on
--            when "0000" => dig <= "0000001";
--            when "0001" => dig <= "0011111";
--            when "0010" => dig <= "0100100";
--            when "0011" => dig <= "0001100";
--            when "0100" => dig <= "0011010";
--            when "0101" => dig <= "1001000";
--            when "0110" => dig <= "1000000";
--            when "0111" => dig <= "0011101";
--            when "1000" => dig <= "0000000";
--            when "1001" => dig <= "0001000";
--            when "1110" => dig <= "0110100";   -- positive sign case
--            when "1111" => dig <= "0101011";   -- negative sign case
--            when others => dig <= (others => 'Z');
            
--        end case CASE_ENCODER;
--    end process ENCODER;
--end Blastn;