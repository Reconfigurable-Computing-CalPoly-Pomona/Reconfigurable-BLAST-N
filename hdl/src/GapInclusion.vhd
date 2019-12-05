library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity GapInclusion is
    Generic (
        g_SIZE   : POSITIVE:= 8     -- Max. amount of characters
    );
    Port (
        i_enable : in  STD_LOGIC;
        i_data   : in  STD_LOGIC_VECTOR((g_SIZE * 2) - 1 downto 0);
        i_size   : in  POSITIVE;
        i_index  : in  NATURAL;
        i_count  : in  NATURAL;
        o_query  : out STD_LOGIC_VECTOR((g_SIZE * 3) - 1 downto 0)
    );
end GapInclusion;

architecture Blastn of GapInclusion is

    signal temp       : STD_LOGIC_VECTOR((g_size * 3) - 1 downto 0);
    signal temp_index : NATURAL:= 0;

begin
    
    CONTROL: process (i_enable) is
    begin

        -- For loops can be used in synthesizable code 
        -- (according to Nandland)
        --GAP_INSERT: for i in 0 to (i_size - 1) generate
            
     
        --end generate GAP_INSERT;

    end process CONTROL;
    
    o_query <= temp;

end Blastn;