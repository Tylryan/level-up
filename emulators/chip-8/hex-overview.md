# Bits, Bytes, and Hexidecimals

## Conversion Table
| **Bits** | **Bytes** | **HEX** |  **Exponential** |**Max Value** |
|----------|-----------|---------|------------------|--------------|
| 1        | 0         | 0x0001  |  2^1 -1          |1             |
| 2        | 0         | 0x0003  |  2^2 -1          |3             |
| 3        | 0         | 0x0007  |  2^3 -1          |7             |
| 4        | 0         | 0x000F  |  2^4 -1          |15            |
| 8        | 1         | 0x00FF  |  2^8 -1          |255           |
| 12       | 1         | 0x0FFF  |  2^12 -1         |4095          |
| 16       | 2         | 0xFFFF  |  2^16 -1         |65535         |

## Conversion Table Explanation
A Hex number contains two portions; a higher region and a lower region. The first two numbers after "0x" are the higher region and the last two are the lower region. Each region is a byte in size.
> E.g. 0x1100 where the two 1s are the higher region and the two 0s are the lower region.  

A Hex value contains (usually as I see it) by default 2 bytes (16 bits) where each position is a nibble (4 bits or half a byte).  
> E.g. 0x000F == 4 bits (1 nibble); 0x00FF == 8 bits (1 byte)  
However, if only two values are present in a hex number, it is usually the lower region.  
> E.g. 0xFF == 0x00FF  

## Easy Hex to Dec Conversion
| Ox | **F** | **F** | **F** | **F** | **Total** |
|----|-------|-------|-------|-------|-----------|
| 0x | 61440 | 3840  | 240   | 15    | 65535     |

> 0x000F == 15
> 0xFFFF == 65535
> 0xFF00 == 65280
> 0x00FF == 255

## Application
1. You have a CPU that has an index register that can hold a maximum of 8 bits. How many bytes is this?
	- 1 Byte (8 bits in a byte)  
2. In decimal format, what is the maximum value this register can hold?  
	- 255 (2 ^ 8 -1)  
3. How do you represent this maximum value in Hexidecimal?  
	- 0xFFF OR 0x0FFF  
4. How many bytes is "0x0F00"?
    - 1 full byte with 4 bits left over
5. How many bytes is "0xFFFFFFFF"?
   - 4 bytes (32 bits).


# Unclear concepts
- Stack pointer
