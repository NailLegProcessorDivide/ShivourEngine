#include "ShivourRenderer.h"

#include <vector>

#include "daveTime.h"
//for dds image decoding
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

int triTable[256][16] =
{ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1} };

ShivourRenderer::ShivourRenderer() {
    FILE* rendererOut = stdout;
    ADDLOG(renderLogger.rendererID, "shiv.renderer", rendererOut);
    ADDLOG(renderLogger.rendererShaderID, "shiv.renderer.shader", rendererOut);
    ADDLOG(renderLogger.rendererPositionID, "shiv.renderer.position", rendererOut);
    ADDLOG(renderLogger.rendererCameraID, "shiv.renderer.camera", rendererOut);
    ADDLOG(renderLogger.rendererTextureID, "shiv.renderer.texture", rendererOut);
    ADDLOG(renderLogger.rendererModelID, "shiv.renderer.model", rendererOut);
    ADDLOG(renderLogger.mCubeLoggerID, "shiv.renderer.model.mcube", rendererOut);
    ADDLOG(renderLogger.renderLightingID, "shiv.renderer.lighting", rendererOut);
    //JLOG_DISABLE("shiv.renderer.model");
}

ShivourRenderer::~ShivourRenderer()
{
    int i = 0;//reused to delete all render components
    int objectRef;
    while ((objectRef = shaders.getIt(i++)) != -1) {
        delete shaders.get(objectRef);
        shaders.remove(objectRef);
    }
    JLOG(renderLogger.rendererID, "deleted all remaining shaders\n");
    i = 0;
    while ((objectRef = models.getIt(i++)) != -1) {
        delete models.get(objectRef);
        models.remove(objectRef);
    }
    JLOG(renderLogger.rendererID, "deleted all remaining models\n");
    i = 0;
    while ((objectRef = textures.getIt(i++)) != -1) {
        delete textures.get(objectRef);
        textures.remove(objectRef);
    }
    JLOG(renderLogger.rendererID, "deleted all remaining textures\n");
    i = 0;
    while ((objectRef = cameras.getIt(i++)) != -1) {
        delete cameras.get(objectRef);
        cameras.remove(objectRef);
    }
    JLOG(renderLogger.rendererID, "deleted all remaining cameras\n");
    i = 0;
    while ((objectRef = positions.getIt(i++)) != -1) {
        delete positions.get(objectRef);
        positions.remove(objectRef);
    }
    JLOG(renderLogger.rendererID, "deleted all remaining positions\n");
    i = 0;
    while ((objectRef = lighting.getIt(i++)) != -1) {
        delete lighting.get(objectRef);
        lighting.remove(objectRef);
    }
    JLOG(renderLogger.renderLightingID, "deleted all remaining lights\n");
}

void ShivourRenderer::init() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    JLOG(renderLogger.rendererID, "initialised renderer\n");
   // glGenerateMipmap(GL_TEXTURE_2D);
}

int ShivourRenderer::addShader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
    FILE* vsf = fopen(vertexShaderFile, "rb");
    FILE* fsf = fopen(fragmentShaderFile, "rb");
    if (vsf && fsf) {
        fseek(vsf, 0, SEEK_END);
        long fsize = ftell(vsf);
        fseek(vsf, 0, SEEK_SET);

        char* vertexShaderSource = (char*)malloc(fsize + 1);
        fread(vertexShaderSource, fsize, 1, vsf);
        fclose(vsf);

        vertexShaderSource[fsize] = 0;

        fseek(fsf, 0, SEEK_END);
        fsize = ftell(fsf);
        fseek(fsf, 0, SEEK_SET);

        char* fragmentShaderSource = (char*)malloc(fsize + 1);
        fread(fragmentShaderSource, fsize, 1, fsf);
        fclose(fsf);

        fragmentShaderSource[fsize] = 0;

        GLuint shadProgID = makeShaderFromSource(vertexShaderSource, fragmentShaderSource);

        free(vertexShaderSource);
        free(fragmentShaderSource);

        int shaderID = shaders.add(new ShivourShader(0, shadProgID));

        JLOG(renderLogger.rendererShaderID, "created shader %i: vs = \"%s\" fs = \"%s\"\n", shaderID, vertexShaderFile, fragmentShaderFile);

        return shaderID;
    }
    JLOG(renderLogger.rendererShaderID, "error unable to load shader: vs = \"%s\" fs = \"%s\"\n", vertexShaderFile, fragmentShaderFile);

    return -1;
}


//MYLOG(x, y, c_args("foo") << 12 << "Test");

void ShivourRenderer::removeShader(int shaderID) {
    delete shaders.get(shaderID);
    shaders.remove(shaderID);

    JLOG(renderLogger.rendererShaderID, "removed shader: %i\n", shaderID);
}

int ShivourRenderer::addCamera() 
{
    int id = cameras.add(new ShivourCamera());

    JLOG(renderLogger.rendererCameraID, "added camera: %i\n", id);

    return id;
}

void ShivourRenderer::removeCamera(int camID) {
    delete cameras.get(camID);
    cameras.remove(camID);

    JLOG(renderLogger.rendererCameraID, "removed camera: %i\n", camID);
}

void ShivourRenderer::setCameraPos(int id, float x, float y, float z) {
    cameras.get(id)->setPosition(x, y, z);
}

void ShivourRenderer::setCameraRotation(int id, float x, float y, float z) {
    cameras.get(id)->setRotation(x, y, z);
}

void ShivourRenderer::addCameraPos(int id, float x, float y, float z) {
    cameras.get(id)->addPosition(x, y, z);
}

void ShivourRenderer::addCameraRotation(int id, float x, float y, float z) {
    cameras.get(id)->addRotation(x, y, z);
}


void ShivourRenderer::moveCamera(int id, float x, float y, float z) {
    cameras.get(id)->move(x, y, z);
}

void ShivourRenderer::setCameraProjection(int id, float fov, float aspect, float nearP, float farP) {
    cameras.get(id)->setProjection(fov, aspect, nearP, farP);
}

int ShivourRenderer::addPosition()
{
    int id = positions.add(new ShivourModelPosition());
    JLOG(renderLogger.rendererPositionID, "added position %i\n", id);
    return id;
}

void ShivourRenderer::removePosition(int posID) {
    delete positions.get(posID);
    positions.remove(posID);

    JLOG(renderLogger.rendererPositionID, "removed position: %i\n", posID);
}

void ShivourRenderer::setPositionPos(int id, float x, float y, float z) {
    positions.get(id)->setPosition(x, y, z);
}

void ShivourRenderer::setPositionRotation(int id, float x, float y, float z) {
    positions.get(id)->setRotation(x, y, z);
}

void ShivourRenderer::setPositionScale(int id, float x, float y, float z) {
    positions.get(id)->addScale(x, y, z);
}

void ShivourRenderer::addPositionPos(int id, float x, float y, float z) {
    positions.get(id)->addPosition(x, y, z);
}

void ShivourRenderer::addPositionRotation(int id, float x, float y, float z) {
    positions.get(id)->addRotation(x, y, z);
}

void ShivourRenderer::addPositionScale(int id, float x, float y, float z) {
    positions.get(id)->addScale(x, y, z);
}

void ShivourRenderer::scalePositionScale(int id, float x, float y, float z) {
    positions.get(id)->mulScale(x, y, z);
}

int ShivourRenderer::addModel(const float* data, int triangleCount)
{
    int id = models.add(new ShivourModel(data, triangleCount));
    JLOG(renderLogger.rendererModelID, "added model %i\n", id);
    return id;
}


int ShivourRenderer::addModel(const float* verticies, const float* normals, const float* textureCoords, int triCount) {
    int id = models.add(new ShivourModel(verticies, normals, textureCoords, triCount));
    JLOG(renderLogger.rendererModelID, "added model %i\n", id);
    return id;
}

inline int serialise(int x, int y, int z, int width, int height) {
    return (x * width + y) * height + z;
}


int ShivourRenderer::addModelMarchingSquares(int width, int height, int depth, float resolution, ShivourNoiseFunc* noise) {
    JLOG(renderLogger.mCubeLoggerID, "starting marching cube gen\n");
    Timer t;
    t.init();
    std::vector<glm::vec3> verticies;
    verticies.reserve(width * depth);
    std::vector<glm::vec3> normals;
    float* buffer = new float[width * height * depth];
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int z = 0; z < depth; ++z) {
                buffer[serialise(x, y, z, height, depth)] = noise->noise(x * resolution, y * resolution, z * resolution);
            }
        }
    }
    for (int x = 0; x < width-1; ++x) {
        for (int y = 0; y < height-1; ++y) {
            for (int z = 0; z < depth-1; ++z) {

                float x1y1z1 = buffer[serialise(x  , y  , z  , height, depth)];
                float x1y1z2 = buffer[serialise(x  , y  , z+1, height, depth)];
                float x1y2z1 = buffer[serialise(x  , y+1, z  , height, depth)];
                float x1y2z2 = buffer[serialise(x  , y+1, z+1, height, depth)];
                float x2y1z1 = buffer[serialise(x+1, y  , z  , height, depth)];
                float x2y1z2 = buffer[serialise(x+1, y  , z+1, height, depth)];
                float x2y2z1 = buffer[serialise(x+1, y+1, z  , height, depth)];
                float x2y2z2 = buffer[serialise(x+1, y+1, z+1, height, depth)];

                glm::vec3 vertexCoordinates[12];

                vertexCoordinates[0] = glm::vec3(x1y1z1 / (x1y1z1 - x2y1z1), 0, 0);
                vertexCoordinates[1] = glm::vec3(1, 0, x2y1z1 / (x2y1z1 - x2y1z2));
                vertexCoordinates[2] = glm::vec3(x1y1z2 / (x1y1z2 - x2y1z2), 0, 1);
                vertexCoordinates[3] = glm::vec3(0, 0, x1y1z1 / (x1y1z1 - x1y1z2));

                vertexCoordinates[4] = glm::vec3(x1y2z1 / (x1y2z1 - x2y2z1), 1, 0);
                vertexCoordinates[5] = glm::vec3(1, 1, x2y2z1 / (x2y2z1 - x2y2z2));
                vertexCoordinates[6] = glm::vec3(x1y2z2 / (x1y2z2 - x2y2z2), 1, 1);
                vertexCoordinates[7] = glm::vec3(0, 1, x1y2z1 / (x1y2z1 - x1y2z2));

                vertexCoordinates[8] = glm::vec3(0, x1y1z1 / (x1y1z1 - x1y2z1), 0);
                vertexCoordinates[9] = glm::vec3(1, x2y1z1 / (x2y1z1 - x2y2z1), 0);
                vertexCoordinates[10] = glm::vec3(1, x2y1z2 / (x2y1z2 - x2y2z2), 1);
                vertexCoordinates[11] = glm::vec3(0, x1y1z2 / (x1y1z2 - x1y2z2), 1);

                uint8_t cornerMap = 0;
                cornerMap |= (buffer[serialise(x  , y  , z  , height, depth)] > 0 ? 1 : 0);
                cornerMap |= (buffer[serialise(x+1, y  , z  , height, depth)] > 0 ? 2 : 0);
                cornerMap |= (buffer[serialise(x+1, y  , z+1, height, depth)] > 0 ? 4 : 0);
                cornerMap |= (buffer[serialise(x  , y  , z+1, height, depth)] > 0 ? 8 : 0);
                cornerMap |= (buffer[serialise(x  , y+1, z  , height, depth)] > 0 ? 16 : 0);
                cornerMap |= (buffer[serialise(x+1, y+1, z  , height, depth)] > 0 ? 32 : 0);
                cornerMap |= (buffer[serialise(x+1, y+1, z+1, height, depth)] > 0 ? 64 : 0);
                cornerMap |= (buffer[serialise(x  , y+1, z+1, height, depth)] > 0 ? 128 : 0);

                for (int i = 0; triTable[cornerMap][i] != -1; i += 3) {
                    glm::vec3 v1 = vertexCoordinates[triTable[cornerMap][i    ]];
                    glm::vec3 v2 = vertexCoordinates[triTable[cornerMap][i + 1]];
                    glm::vec3 v3 = vertexCoordinates[triTable[cornerMap][i + 2]];

                    verticies.push_back((glm::vec3(x, y, z) + v1) * resolution);
                    verticies.push_back((glm::vec3(x, y, z) + v2) * resolution);
                    verticies.push_back((glm::vec3(x, y, z) + v3) * resolution);
                }

            }
        }
    }
    normals.reserve(verticies.size());
    for (auto it = verticies.begin(); it != verticies.end();) {

        glm::vec3& v1 = *(it++);
        glm::vec3& v2 = *(it++);
        glm::vec3& v3 = *(it++);

        glm::vec3 normal = glm::normalize(glm::cross(v1 - v2, v1 - v3));
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
    }

    delete[] buffer;
    float* texcos = new float[verticies.size() * 2 * 3];
    int out = models.add(new ShivourModel((float*)verticies.data(), (float*)normals.data(), texcos, verticies.size()/3));
    delete[] texcos;

    JLOG(renderLogger.mCubeLoggerID, "generated model %d: %d verts, %f secs\n", out, verticies.size() / 3, t.getElapsedTime());

    return out;
}

int ShivourRenderer::addModel(const char* fileName)
{
    int id = models.add(new ShivourModel(fileName));
    JLOG(renderLogger.rendererModelID, "added model %i\n", id);
    return id;
}

void ShivourRenderer::removeModel(int modelID) {
    delete models.get(modelID);
    models.remove(modelID);
    JLOG(renderLogger.rendererModelID, "removed model %i\n", modelID);
}

int ShivourRenderer::addTextures() {
    int id = textures.add(new ShivourTextures());
    JLOG(renderLogger.rendererTextureID, "added texture %i\n", id);
    return id;
}
int ShivourRenderer::addTextures(const char* fileName) {
    int id = textures.add(new ShivourTextures(fileName));
    JLOG(renderLogger.rendererTextureID, "added texture %i\n", id);
    return id;
}

void ShivourRenderer::removeTextures(int textureID) {
    delete textures.get(textureID);
    textures.remove(textureID);
    JLOG(renderLogger.rendererTextureID, "removed texture %i\n", textureID);
}

GLuint ShivourRenderer::makeShaderFromSource(const char* vs, const char* fs)
{
    GLint compileStatus;
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vs, nullptr);
    glCompileShader(vertexShaderID);
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        JLOG(renderLogger.rendererShaderID, "compile error vs\n");
        GLint maxLength = 0;
        glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertexShaderID, maxLength, &maxLength, &errorLog[0]);
        printf(&errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(vertexShaderID); // Don't leak the shader.
        return -1;
    }

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fs, nullptr);
    glCompileShader(fragmentShaderID);
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        JLOG(renderLogger.rendererShaderID, "compile error fs\n");
        GLint maxLength = 0;
        glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragmentShaderID, maxLength, &maxLength, &errorLog[0]);
        printf(&errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(fragmentShaderID); // Don't leak the shader.
        return -1;
    }

    GLuint shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);
    glLinkProgram(shaderProgramID);
    glDetachShader(shaderProgramID, vertexShaderID);
    glDetachShader(shaderProgramID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    GLchar* infoLog = new GLchar[1024];
    GLint length;
    glGetProgramInfoLog(shaderProgramID, 1023, &length, infoLog);
    //printf("program info log %d : %s\n", shaderProgramID, infoLog);
    delete[] infoLog;

    return shaderProgramID;
}

int ShivourRenderer::addLightingScene(float x, float y, float z, float r, float g, float b) {
    int id = lighting.add(new ShivourLights(glm::vec3(x, y, z), glm::vec3(r, g, b)));
    JLOG(renderLogger.renderLightingID, "adding light scene: %d\n", id);
    return id;
}

void ShivourRenderer::removeLightingScene(int id) {
    delete lighting.get(id);
    lighting.remove(id);
    JLOG(renderLogger.renderLightingID, "removed light scene: %d\n", id);
}

void ShivourRenderer::render(std::set<ShivourRenderable, ShivourRenderableCompare>& renderables) {
    int shader = -1;
    int camera = -1;
    int model = -1;
    int texture = -1;
    int positionData = -1;
    int lightingData = -1;
    uint32_t numTris;
    ShivourShader* currentShader = nullptr;
    for (auto& sr : renderables) {
        if (sr.shader != shader && sr.shader != -1) {
            currentShader = shaders.get(sr.shader);
            currentShader->bind();
            shader = sr.shader;
        }
        if (sr.model != model && sr.model != -1) {
            models.get(sr.model)->load();
            model = sr.model;
            numTris = models.get(sr.model)->getNumTris();
        }
        if (sr.texture != texture && sr.texture != -1) {
            textures.get(sr.texture)->bind(currentShader);
            texture = sr.texture;
        }
        if (sr.camera != camera && sr.camera != -1) {
            cameras.get(sr.camera)->load(currentShader);
            camera = sr.camera;
        }
        if (sr.positionData != positionData && sr.positionData != -1) {
            positions.get(sr.positionData)->load(currentShader);
            positionData = sr.positionData;
        }
        if (sr.lighting != lightingData && sr.lighting != -1) {
            lighting.get(sr.lighting)->load(currentShader);
            lightingData = sr.lighting;
        }
        //printf("render: %d, %d, %d, %d, %d\n", shader, model, camera, positionData, numTris);
        glDrawArrays(GL_TRIANGLES, 0, numTris*3);
    }
    glUseProgram(NULL);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

ShivourShader::ShivourShader(uint32_t shadInfo, GLuint prog) :
    shaderInfo(shadInfo), program(prog),
    modelUniform(glGetUniformLocation(prog, "modelMatrix")),
    cameraUniform(glGetUniformLocation(prog, "cameraMatrix")),
    camPos(glGetUniformLocation(prog, "camPos")),
    lightPos(glGetUniformLocation(prog, "lightPos")),
    lightCol(glGetUniformLocation(prog, "lightCol"))
{
}

ShivourShader::~ShivourShader() {

}

bool ShivourRenderableCompare::operator() (const ShivourRenderable& left,
    const ShivourRenderable& right)const
{
    if (left.shader > right.shader) {
        return true;
    }
    if (left.shader < right.shader) {
        return false;
    }
    if (left.model > right.model) {
        return true;
    }
    if (left.model < right.model) {
        return false;
    }
    if (left.texture > right.texture) {
        return true;
    }
    if (left.texture < right.texture) {
        return false;
    }
    if (left.camera > right.camera) {
        return true;
    }
    if (left.camera < right.camera) {
        return false;
    }
    if (left.positionData > right.positionData) {
        return true;
    }
    if (left.positionData < right.positionData) {
        return false;
    }
    if (left.lighting > right.lighting) {
        return true;
    }
    if (left.lighting < right.lighting) {
        return false;
    }
    return false;
}

void ShivourShader::bind()
{
    glUseProgram(program);
    //printf("bind shader %d\n", program);
}

ShivourTextures::ShivourTextures(): textureID(0) 
{}

ShivourTextures::ShivourTextures(const char* fileName) : textureID(readTexture(fileName)) 
{
}

ShivourTextures::~ShivourTextures() {}

//https://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
GLuint ShivourTextures::readTexture(const char* fileName) {
    unsigned char header[124];

    FILE* fp;

    /* try to open the file */
    fp = fopen(fileName, "rb");
    if (fp == NULL)
        return 0;

    /* verify the type of file */
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        return 0;
    }

    /* get the surface desc */
    fread(&header, 124, 1, fp);

    unsigned int height = *(unsigned int*)&(header[8]);
    unsigned int width = *(unsigned int*)&(header[12]);
    unsigned int linearSize = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC = *(unsigned int*)&(header[80]);
    unsigned char* buffer;
    unsigned int bufsize;
    /* how big is it going to be including all mipmaps? */
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    /* close the file pointer */
    fclose(fp);

    unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch (fourCC)
    {
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    default:
        free(buffer);
        return 0;
    }

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    /* load the mipmaps */
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
            0, size, buffer + offset);

        offset += size;
        width /= 2;
        height /= 2;
    }
    free(buffer);

    return textureID;
}

void ShivourTextures::bind(ShivourShader* currentShader) {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

ShivourModel::ShivourModel(const float* data, int tricount) {
    GLfloat* zeroBuffer = new GLfloat[tricount * 9];
    memset(zeroBuffer, 0, sizeof(float) * tricount * 9);
    glGenBuffers(3, (GLuint*)&modelData.buffers);
    \
    glBindBuffer(GL_ARRAY_BUFFER, modelData.buffers.vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, tricount * 9 * sizeof(GLfloat), data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, modelData.buffers.normalBufferID);
    glBufferData(GL_ARRAY_BUFFER, tricount * 9 * sizeof(GLfloat), zeroBuffer, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, modelData.buffers.texBufferID);
    glBufferData(GL_ARRAY_BUFFER, tricount * 6 * sizeof(GLfloat), zeroBuffer, GL_STATIC_DRAW);

    modelData.triCount = tricount;
}

ShivourModel::ShivourModel(const float* verticies, const float* normals, const float* textureCoords, int tricount) {

    glGenBuffers(3, (GLuint*)&modelData.buffers);

    glBindBuffer(GL_ARRAY_BUFFER, modelData.buffers.vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, tricount * 9 * sizeof(GLfloat), verticies, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, modelData.buffers.normalBufferID);
    glBufferData(GL_ARRAY_BUFFER, tricount * 9 * sizeof(GLfloat), normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, modelData.buffers.texBufferID);
    glBufferData(GL_ARRAY_BUFFER, tricount * 6 * sizeof(GLfloat), textureCoords, GL_STATIC_DRAW);

    modelData.triCount = tricount;
}

ShivourModel::ShivourModel(const char* fileName) : modelData(){
    FILE* objectFile = fopen(fileName, "rb");

    std::vector<float> vertex;
    std::vector<float> vertexNorm;
    std::vector<float> vertexTex;
    std::vector<float> vertexOut;
    std::vector<float> vertexNormOut;
    std::vector<float> vertexTexOut;
    if (objectFile) {
        char line[100];
        int i;
        while ((i = fscanf(objectFile, "%99[^\n]\n", line)) > 0) {
            float vx, vy, vz;
            int pxv, pxt, pxn, pyv, pyt, pyn, pzv, pzt, pzn;

            if (sscanf(line, "v %f %f %f", &vx, &vy, &vz) == 3) {
                //printf("v %f %f %f\n", vx, vy, vz);
                vertex.push_back(vx);
                vertex.push_back(vy);
                vertex.push_back(vz);
            }
            else if (sscanf(line, "vt %f %f", &vx, &vy) == 2) {
                //printf("t %f %f\n", vx, vy);
                vertexTex.push_back(vx);
                vertexTex.push_back(vy);
            }
            else if (sscanf(line, "vn %f %f %f", &vx, &vy, &vz) == 3) {
                //printf("n %f %f %f\n", vx, vy, vz);
                vertexNorm.push_back(vx);
                vertexNorm.push_back(vy);
                vertexNorm.push_back(vz);
            }
            else if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &pxv, &pxt, &pxn, &pyv, &pyt, &pyn, &pzv, &pzt, &pzn) == 9) {
                //printf("f %i/%i/%i %i/%i/%i %i/%i/%i\n", pxv, pxt, pxn, pyv, pyt, pyn, pzv, pzt, pzn);
                vertexOut.push_back(vertex.at(pxv * 3 - 3));//things be 1 indexed they do
                vertexOut.push_back(vertex.at(pxv * 3 - 2));
                vertexOut.push_back(vertex.at(pxv * 3 - 1));
                vertexOut.push_back(vertex.at(pyv * 3 - 3));
                vertexOut.push_back(vertex.at(pyv * 3 - 2));
                vertexOut.push_back(vertex.at(pyv * 3 - 1));
                vertexOut.push_back(vertex.at(pzv * 3 - 3));
                vertexOut.push_back(vertex.at(pzv * 3 - 2));
                vertexOut.push_back(vertex.at(pzv * 3 - 1));

                vertexTexOut.push_back(vertexTex.at(pxt * 2 - 2));
                vertexTexOut.push_back(vertexTex.at(pxt * 2 - 1));
                vertexTexOut.push_back(vertexTex.at(pyt * 2 - 2));
                vertexTexOut.push_back(vertexTex.at(pyt * 2 - 1));
                vertexTexOut.push_back(vertexTex.at(pzt * 2 - 2));
                vertexTexOut.push_back(vertexTex.at(pzt * 2 - 1));

                vertexNormOut.push_back(vertexNorm.at(pxn * 3 - 3));
                vertexNormOut.push_back(vertexNorm.at(pxn * 3 - 2));
                vertexNormOut.push_back(vertexNorm.at(pxn * 3 - 1));
                vertexNormOut.push_back(vertexNorm.at(pyn * 3 - 3));
                vertexNormOut.push_back(vertexNorm.at(pyn * 3 - 2));
                vertexNormOut.push_back(vertexNorm.at(pyn * 3 - 1));
                vertexNormOut.push_back(vertexNorm.at(pzn * 3 - 3));
                vertexNormOut.push_back(vertexNorm.at(pzn * 3 - 2));
                vertexNormOut.push_back(vertexNorm.at(pzn * 3 - 1));
            }
        }
        fclose(objectFile);
    }
    else {

    }
    glGenBuffers(3, (GLuint*)&modelData.buffers);

    glBindBuffer(GL_ARRAY_BUFFER, modelData.buffers.vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertexOut.size() * sizeof(GLfloat), vertexOut.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, modelData.buffers.normalBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertexNormOut.size() * sizeof(GLfloat), vertexNormOut.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, modelData.buffers.texBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertexTexOut.size() * sizeof(GLfloat), vertexTexOut.data(), GL_STATIC_DRAW);

    modelData.triCount = vertexOut.size()/9;
}

ShivourModel::~ShivourModel() {

}

void ShivourModel::load() {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, modelData.buffers.vertexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, modelData.buffers.normalBufferID);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, modelData.buffers.texBufferID);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

uint32_t ShivourModel::getNumTris() {
    return modelData.triCount;
}

ShivourCamera::ShivourCamera() : position(0,0,0), rotation(0,0,0,1),
        projection(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1){
    
}

ShivourCamera::~ShivourCamera() {

}

void ShivourCamera::load(ShivourShader* currentShader) {
    glm::mat4 rotationMat = glm::toMat4(-rotation);
    glm::mat4 positionMat = { 1,0,0,0,0,1,0,0,0,0,1,0,-position.x,-position.y,-position.z,1 };
    glm::mat4 camMat = projection * rotationMat * positionMat;
    glUniformMatrix4fv(currentShader->cameraUniform, 1, GL_FALSE, glm::value_ptr(camMat));
    glUniform3fv(currentShader->camPos, 1, glm::value_ptr(position));
}

void ShivourCamera::setPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

void ShivourCamera::setRotation(float x, float y, float z) {
    float sx = sin(x / 2); float sy = sin(y / 2); float sz = sin(z / 2);
    float cx = cos(x / 2); float cy = cos(y / 2); float cz = cos(z / 2);

    rotation = glm::quat(cx * cy * cz - sx * sy * sz,
        sx * cy * cz + cx * sy * sz,
        cx * sy * cz - sx * cy * sz,
        cx * cy * sz + sx * sy * cz);
}

void ShivourCamera::addPosition(float x, float y, float z) {
    position.x += x;
    position.y += y;
    position.z += z;
}

void ShivourCamera::addRotation(float x, float y, float z) {
    float sx = sin(x / 2); float sy = sin(y / 2); float sz = sin(z / 2);
    float cx = cos(x / 2); float cy = cos(y / 2); float cz = cos(z / 2);

    rotation = glm::quat(cx * cy * cz - sx * sy * sz,
        sx * cy * cz + cx * sy * sz,
        cx * sy * cz - sx * cy * sz,
        cx * cy * sz + sx * sy * cz) * rotation;
}

void ShivourCamera::move(float x, float y, float z) {
    glm::vec3 offset = rotation * glm::vec3(x, y, z);
    //offset.y *= -1;
    position += offset;
}

void ShivourCamera::setProjection(float fov, float aspectRatio, float nearPlane, float farPlane) {
    projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
    //printf("%s\n", glm::to_string(projection).c_str());
}

ShivourModelPosition::ShivourModelPosition() : position(0,0,0), rotation(0,0,0,1), scale(1,1,1) 
{

}

void ShivourModelPosition::load(ShivourShader* currentShader) {
    glm::mat4 scaleMat = { scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1 };
    glm::mat4 rotationMat = glm::toMat4(rotation);
    glm::mat4 positionMat = { 1,0,0,0,0,1,0,0,0,0,1,0,position.x,position.y,position.z,1 };
    glm::mat4 modelMat = positionMat * rotationMat * scaleMat;
    glUniformMatrix4fv(currentShader->modelUniform, 1, GL_FALSE, glm::value_ptr(modelMat));
}

void ShivourModelPosition::setPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

void ShivourModelPosition::addPosition(float x, float y, float z) {
    position.x += x;
    position.y += y;
    position.z += z;
}

void ShivourModelPosition::setRotation(float x, float y, float z) {
    float sx = sin(x / 2); float sy = sin(y / 2); float sz = sin(z / 2);
    float cx = cos(x / 2); float cy = cos(y / 2); float cz = cos(z / 2);

    rotation = glm::quat(cx * cy * cz - sx * sy * sz,
        sx * cy * cz + cx * sy * sz,
        cx * sy * cz - sx * cy * sz,
        cx * cy * sz + sx * sy * cz);
}

void ShivourModelPosition::addRotation(float x, float y, float z) {
    float sx = sin(x / 2); float sy = sin(y / 2); float sz = sin(z / 2);
    float cx = cos(x / 2); float cy = cos(y / 2); float cz = cos(z / 2);

    rotation = glm::quat(cx * cy * cz - sx * sy * sz,
        sx * cy * cz + cx * sy * sz,
        cx * sy * cz - sx * cy * sz,
        cx * cy * sz + sx * sy * cz) * rotation;
}

void ShivourModelPosition::setScale(float x, float y, float z) {
    scale.x = x;
    scale.y = y;
    scale.z = z;
}

void ShivourModelPosition::addScale(float x, float y, float z) {
    scale.x += x;
    scale.y += y;
    scale.z += z;
}

void ShivourModelPosition::mulScale(float x, float y, float z) {
    scale.x *= x;
    scale.y *= y;
    scale.z *= z;
}

ShivourLights::ShivourLights(glm::vec3 pos, glm::vec3 col) : light{pos, col} {

}

ShivourLights::~ShivourLights() {

}

void ShivourLights::load(ShivourShader* shader) {

    glUniform3fv(shader->lightPos, 1, glm::value_ptr(light.lightPos));
    glUniform3fv(shader->lightCol, 1, glm::value_ptr(light.lightCol));
}