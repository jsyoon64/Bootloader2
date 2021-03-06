/**
  ******************************************************************************
  * @file    flash_layout.c
  * @author  jsyoon
  * @brief   Contains Flash layout
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "platform.h"

#include "openbootloader_conf.h"
#include "flash_layout.h"

/* Private typedef -----------------------------------------------------------*/
/** Flash sector descriptor type. */
typedef struct
{
  uint32_t  sector_start; /**< sector start address             */
  uint32_t  sector_size;  /**< sector size in bytes             */
  uint16_t  sector_num;   /**< sector number                    */
} tFlashSector;

/* Private define ------------------------------------------------------------*/
/** Value for an invalid sector entry index into flashLayout[]. */
#define FLASH_INVALID_SECTOR_IDX        (0xffff)
/** Value for an invalid flash address. */
#define FLASH_INVALID_ADDRESS           (0xffffffff)

#if 0
/** Total numbers of sectors in array flashLayout[]. */
#define FLASH_TOTAL_SECTORS             (sizeof(flashLayout)/sizeof(flashLayout[0]))
/** End address of the bootloader programmable flash. */
#endif

#if 0
#define FLASH_END_ADDRESS               (flashLayout[FLASH_TOTAL_SECTORS-1].sector_start + \
                                         flashLayout[FLASH_TOTAL_SECTORS-1].sector_size - 1)
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if 0
static const tFlashSector flashLayout[] =
{
  /* space is reserved for a bootloader configuration with all supported communication
   * interfaces enabled. when for example only UART is needed, than the space required
   * for the bootloader can be made a lot smaller here.
   */
  #if 0
  /* { 0x08000000, 0x04000,  0},           flash sector  0 - reserved for bootloader   */
  /* { 0x08004000, 0x04000,  1},           flash sector  1 - reserved for bootloader   */
  { 0x08008000, 0x04000,  2},           /* flash sector  2 -  16kb                     */
  { 0x0800c000, 0x04000,  3},           /* flash sector  3 -  16kb                     */
  { 0x08010000, 0x10000,  4},           /* flash sector  4 -  64kb                     */
  { 0x08020000, 0x20000,  5},           /* flash sector  5 - 128kb                     */
  { 0x08040000, 0x20000,  6},           /* flash sector  6 - 128kb                     */
  { 0x08060000, 0x20000,  7},           /* flash sector  7 - 128kb                     */
  { 0x08080000, 0x20000,  8},           /* flash sector  8 - 128kb                     */
  { 0x080A0000, 0x20000,  9},           /* flash sector  9 - 128kb                     */
  { 0x080C0000, 0x20000, 10},           /* flash sector 10 - 128kb                     */
  { 0x080E0000, 0x20000, 11},           /* flash sector 11 - 128kb                     */
  #else
  /* reserved for bootloader */
  /*
  { 0x08000000, 0x00100,  0},
  { 0x08000100, 0x00100,  1},
  { 0x08000200, 0x00100,  2},
  { 0x08000300, 0x00100,  3},
  { 0x08000400, 0x00100,  4},
  { 0x08000500, 0x00100,  5},
  { 0x08000600, 0x00100,  6},
  { 0x08000700, 0x00100,  7},
  { 0x08000800, 0x00100,  8},
  { 0x08000900, 0x00100,  9},
  { 0x08000A00, 0x00100,  10},
  { 0x08000B00, 0x00100,  11},
  { 0x08000C00, 0x00100,  12},
  { 0x08000D00, 0x00100,  13},
  { 0x08000E00, 0x00100,  14},
  { 0x08000F00, 0x00100,  15},
  { 0x08001000, 0x00100,  16},
  { 0x08001100, 0x00100,  17},
  { 0x08001200, 0x00100,  18},
  { 0x08001300, 0x00100,  19},
  { 0x08001400, 0x00100,  20},
  { 0x08001500, 0x00100,  21},
  { 0x08001600, 0x00100,  22},
  { 0x08001700, 0x00100,  23},
  { 0x08001800, 0x00100,  24},
  { 0x08001900, 0x00100,  25},
  { 0x08001A00, 0x00100,  26},
  { 0x08001B00, 0x00100,  27},
  { 0x08001C00, 0x00100,  28},
  { 0x08001D00, 0x00100,  29},
  { 0x08001E00, 0x00100,  30},
  { 0x08001F00, 0x00100,  31},
  { 0x08002000, 0x00100,  32},
  { 0x08002100, 0x00100,  33},
  { 0x08002200, 0x00100,  34},
  { 0x08002300, 0x00100,  35},
  { 0x08002400, 0x00100,  36},
  { 0x08002500, 0x00100,  37},
  { 0x08002600, 0x00100,  38},
  { 0x08002700, 0x00100,  39},
  { 0x08002800, 0x00100,  40},
  { 0x08002900, 0x00100,  41},
  { 0x08002A00, 0x00100,  42},
  { 0x08002B00, 0x00100,  43},
  { 0x08002C00, 0x00100,  44},
  { 0x08002D00, 0x00100,  45},
  { 0x08002E00, 0x00100,  46},
  { 0x08002F00, 0x00100,  47},
  { 0x08003000, 0x00100,  48},
  { 0x08003100, 0x00100,  49},
  { 0x08003200, 0x00100,  50},
  { 0x08003300, 0x00100,  51},
  { 0x08003400, 0x00100,  52},
  { 0x08003500, 0x00100,  53},
  { 0x08003600, 0x00100,  54},
  { 0x08003700, 0x00100,  55},
  { 0x08003800, 0x00100,  56},
  { 0x08003900, 0x00100,  57},
  { 0x08003A00, 0x00100,  58},
  { 0x08003B00, 0x00100,  59},
  { 0x08003C00, 0x00100,  60},
  { 0x08003D00, 0x00100,  61},
  { 0x08003E00, 0x00100,  62},
  { 0x08003F00, 0x00100,  63},
  { 0x08004000, 0x00100,  64},
  { 0x08004100, 0x00100,  65},
  { 0x08004200, 0x00100,  66},
  { 0x08004300, 0x00100,  67},
  { 0x08004400, 0x00100,  68},
  { 0x08004500, 0x00100,  69},
  { 0x08004600, 0x00100,  70},
  { 0x08004700, 0x00100,  71},
  { 0x08004800, 0x00100,  72},
  { 0x08004900, 0x00100,  73},
  { 0x08004A00, 0x00100,  74},
  { 0x08004B00, 0x00100,  75},
  { 0x08004C00, 0x00100,  76},
  { 0x08004D00, 0x00100,  77},
  { 0x08004E00, 0x00100,  78},
  { 0x08004F00, 0x00100,  79},
  { 0x08005000, 0x00100,  80},
  { 0x08005100, 0x00100,  81},
  { 0x08005200, 0x00100,  82},
  { 0x08005300, 0x00100,  83},
  { 0x08005400, 0x00100,  84},
  { 0x08005500, 0x00100,  85},
  { 0x08005600, 0x00100,  86},
  { 0x08005700, 0x00100,  87},
  { 0x08005800, 0x00100,  88},
  { 0x08005900, 0x00100,  89},
  { 0x08005A00, 0x00100,  90},
  { 0x08005B00, 0x00100,  91},
  { 0x08005C00, 0x00100,  92},
  { 0x08005D00, 0x00100,  93},
  { 0x08005E00, 0x00100,  94},
  { 0x08005F00, 0x00100,  95},
  { 0x08006000, 0x00100,  96},
  { 0x08006100, 0x00100,  97},
  { 0x08006200, 0x00100,  98},
  { 0x08006300, 0x00100,  99},
  { 0x08006400, 0x00100,  100},
  { 0x08006500, 0x00100,  101},
  { 0x08006600, 0x00100,  102},
  { 0x08006700, 0x00100,  103},
  { 0x08006800, 0x00100,  104},
  { 0x08006900, 0x00100,  105},
  { 0x08006A00, 0x00100,  106},
  { 0x08006B00, 0x00100,  107},
  { 0x08006C00, 0x00100,  108},
  { 0x08006D00, 0x00100,  109},
  { 0x08006E00, 0x00100,  110},
  { 0x08006F00, 0x00100,  111},
  { 0x08007000, 0x00100,  112},
  { 0x08007100, 0x00100,  113},
  { 0x08007200, 0x00100,  114},
  { 0x08007300, 0x00100,  115},
  { 0x08007400, 0x00100,  116},
  { 0x08007500, 0x00100,  117},
  { 0x08007600, 0x00100,  118},
  { 0x08007700, 0x00100,  119},
  { 0x08007800, 0x00100,  120},
  { 0x08007900, 0x00100,  121},
  { 0x08007A00, 0x00100,  122},
  { 0x08007B00, 0x00100,  123},
  { 0x08007C00, 0x00100,  124},
  { 0x08007D00, 0x00100,  125},
  { 0x08007E00, 0x00100,  126},
  { 0x08007F00, 0x00100,  127},
*/
  { 0x08008000, 0x00100,  128},
  { 0x08008100, 0x00100,  129},
  { 0x08008200, 0x00100,  130},
  { 0x08008300, 0x00100,  131},
  { 0x08008400, 0x00100,  132},
  { 0x08008500, 0x00100,  133},
  { 0x08008600, 0x00100,  134},
  { 0x08008700, 0x00100,  135},
  { 0x08008800, 0x00100,  136},
  { 0x08008900, 0x00100,  137},
  { 0x08008A00, 0x00100,  138},
  { 0x08008B00, 0x00100,  139},
  { 0x08008C00, 0x00100,  140},
  { 0x08008D00, 0x00100,  141},
  { 0x08008E00, 0x00100,  142},
  { 0x08008F00, 0x00100,  143},
  { 0x08009000, 0x00100,  144},
  { 0x08009100, 0x00100,  145},
  { 0x08009200, 0x00100,  146},
  { 0x08009300, 0x00100,  147},
  { 0x08009400, 0x00100,  148},
  { 0x08009500, 0x00100,  149},
  { 0x08009600, 0x00100,  150},
  { 0x08009700, 0x00100,  151},
  { 0x08009800, 0x00100,  152},
  { 0x08009900, 0x00100,  153},
  { 0x08009A00, 0x00100,  154},
  { 0x08009B00, 0x00100,  155},
  { 0x08009C00, 0x00100,  156},
  { 0x08009D00, 0x00100,  157},
  { 0x08009E00, 0x00100,  158},
  { 0x08009F00, 0x00100,  159},
  { 0x0800A000, 0x00100,  160},
  { 0x0800A100, 0x00100,  161},
  { 0x0800A200, 0x00100,  162},
  { 0x0800A300, 0x00100,  163},
  { 0x0800A400, 0x00100,  164},
  { 0x0800A500, 0x00100,  165},
  { 0x0800A600, 0x00100,  166},
  { 0x0800A700, 0x00100,  167},
  { 0x0800A800, 0x00100,  168},
  { 0x0800A900, 0x00100,  169},
  { 0x0800AA00, 0x00100,  170},
  { 0x0800AB00, 0x00100,  171},
  { 0x0800AC00, 0x00100,  172},
  { 0x0800AD00, 0x00100,  173},
  { 0x0800AE00, 0x00100,  174},
  { 0x0800AF00, 0x00100,  175},
  { 0x0800B000, 0x00100,  176},
  { 0x0800B100, 0x00100,  177},
  { 0x0800B200, 0x00100,  178},
  { 0x0800B300, 0x00100,  179},
  { 0x0800B400, 0x00100,  180},
  { 0x0800B500, 0x00100,  181},
  { 0x0800B600, 0x00100,  182},
  { 0x0800B700, 0x00100,  183},
  { 0x0800B800, 0x00100,  184},
  { 0x0800B900, 0x00100,  185},
  { 0x0800BA00, 0x00100,  186},
  { 0x0800BB00, 0x00100,  187},
  { 0x0800BC00, 0x00100,  188},
  { 0x0800BD00, 0x00100,  189},
  { 0x0800BE00, 0x00100,  190},
  { 0x0800BF00, 0x00100,  191},
  { 0x0800C000, 0x00100,  192},
  { 0x0800C100, 0x00100,  193},
  { 0x0800C200, 0x00100,  194},
  { 0x0800C300, 0x00100,  195},
  { 0x0800C400, 0x00100,  196},
  { 0x0800C500, 0x00100,  197},
  { 0x0800C600, 0x00100,  198},
  { 0x0800C700, 0x00100,  199},
  { 0x0800C800, 0x00100,  200},
  { 0x0800C900, 0x00100,  201},
  { 0x0800CA00, 0x00100,  202},
  { 0x0800CB00, 0x00100,  203},
  { 0x0800CC00, 0x00100,  204},
  { 0x0800CD00, 0x00100,  205},
  { 0x0800CE00, 0x00100,  206},
  { 0x0800CF00, 0x00100,  207},
  { 0x0800D000, 0x00100,  208},
  { 0x0800D100, 0x00100,  209},
  { 0x0800D200, 0x00100,  210},
  { 0x0800D300, 0x00100,  211},
  { 0x0800D400, 0x00100,  212},
  { 0x0800D500, 0x00100,  213},
  { 0x0800D600, 0x00100,  214},
  { 0x0800D700, 0x00100,  215},
  { 0x0800D800, 0x00100,  216},
  { 0x0800D900, 0x00100,  217},
  { 0x0800DA00, 0x00100,  218},
  { 0x0800DB00, 0x00100,  219},
  { 0x0800DC00, 0x00100,  220},
  { 0x0800DD00, 0x00100,  221},
  { 0x0800DE00, 0x00100,  222},
  { 0x0800DF00, 0x00100,  223},
  { 0x0800E000, 0x00100,  224},
  { 0x0800E100, 0x00100,  225},
  { 0x0800E200, 0x00100,  226},
  { 0x0800E300, 0x00100,  227},
  { 0x0800E400, 0x00100,  228},
  { 0x0800E500, 0x00100,  229},
  { 0x0800E600, 0x00100,  230},
  { 0x0800E700, 0x00100,  231},
  { 0x0800E800, 0x00100,  232},
  { 0x0800E900, 0x00100,  233},
  { 0x0800EA00, 0x00100,  234},
  { 0x0800EB00, 0x00100,  235},
  { 0x0800EC00, 0x00100,  236},
  { 0x0800ED00, 0x00100,  237},
  { 0x0800EE00, 0x00100,  238},
  { 0x0800EF00, 0x00100,  239},
  { 0x0800F000, 0x00100,  240},
  { 0x0800F100, 0x00100,  241},
  { 0x0800F200, 0x00100,  242},
  { 0x0800F300, 0x00100,  243},
  { 0x0800F400, 0x00100,  244},
  { 0x0800F500, 0x00100,  245},
  { 0x0800F600, 0x00100,  246},
  { 0x0800F700, 0x00100,  247},
  { 0x0800F800, 0x00100,  248},
  { 0x0800F900, 0x00100,  249},
  { 0x0800FA00, 0x00100,  250},
  { 0x0800FB00, 0x00100,  251},
  { 0x0800FC00, 0x00100,  252},
  { 0x0800FD00, 0x00100,  253},
  { 0x0800FE00, 0x00100,  254},
  { 0x0800FF00, 0x00100,  255},
  { 0x08010000, 0x00100,  256},
  { 0x08010100, 0x00100,  257},
  { 0x08010200, 0x00100,  258},
  { 0x08010300, 0x00100,  259},
  { 0x08010400, 0x00100,  260},
  { 0x08010500, 0x00100,  261},
  { 0x08010600, 0x00100,  262},
  { 0x08010700, 0x00100,  263},
  { 0x08010800, 0x00100,  264},
  { 0x08010900, 0x00100,  265},
  { 0x08010A00, 0x00100,  266},
  { 0x08010B00, 0x00100,  267},
  { 0x08010C00, 0x00100,  268},
  { 0x08010D00, 0x00100,  269},
  { 0x08010E00, 0x00100,  270},
  { 0x08010F00, 0x00100,  271},
  { 0x08011000, 0x00100,  272},
  { 0x08011100, 0x00100,  273},
  { 0x08011200, 0x00100,  274},
  { 0x08011300, 0x00100,  275},
  { 0x08011400, 0x00100,  276},
  { 0x08011500, 0x00100,  277},
  { 0x08011600, 0x00100,  278},
  { 0x08011700, 0x00100,  279},
  { 0x08011800, 0x00100,  280},
  { 0x08011900, 0x00100,  281},
  { 0x08011A00, 0x00100,  282},
  { 0x08011B00, 0x00100,  283},
  { 0x08011C00, 0x00100,  284},
  { 0x08011D00, 0x00100,  285},
  { 0x08011E00, 0x00100,  286},
  { 0x08011F00, 0x00100,  287},
  { 0x08012000, 0x00100,  288},
  { 0x08012100, 0x00100,  289},
  { 0x08012200, 0x00100,  290},
  { 0x08012300, 0x00100,  291},
  { 0x08012400, 0x00100,  292},
  { 0x08012500, 0x00100,  293},
  { 0x08012600, 0x00100,  294},
  { 0x08012700, 0x00100,  295},
  { 0x08012800, 0x00100,  296},
  { 0x08012900, 0x00100,  297},
  { 0x08012A00, 0x00100,  298},
  { 0x08012B00, 0x00100,  299},
  { 0x08012C00, 0x00100,  300},
  { 0x08012D00, 0x00100,  301},
  { 0x08012E00, 0x00100,  302},
  { 0x08012F00, 0x00100,  303},
  { 0x08013000, 0x00100,  304},
  { 0x08013100, 0x00100,  305},
  { 0x08013200, 0x00100,  306},
  { 0x08013300, 0x00100,  307},
  { 0x08013400, 0x00100,  308},
  { 0x08013500, 0x00100,  309},
  { 0x08013600, 0x00100,  310},
  { 0x08013700, 0x00100,  311},
  { 0x08013800, 0x00100,  312},
  { 0x08013900, 0x00100,  313},
  { 0x08013A00, 0x00100,  314},
  { 0x08013B00, 0x00100,  315},
  { 0x08013C00, 0x00100,  316},
  { 0x08013D00, 0x00100,  317},
  { 0x08013E00, 0x00100,  318},
  { 0x08013F00, 0x00100,  319},
  { 0x08014000, 0x00100,  320},
  { 0x08014100, 0x00100,  321},
  { 0x08014200, 0x00100,  322},
  { 0x08014300, 0x00100,  323},
  { 0x08014400, 0x00100,  324},
  { 0x08014500, 0x00100,  325},
  { 0x08014600, 0x00100,  326},
  { 0x08014700, 0x00100,  327},
  { 0x08014800, 0x00100,  328},
  { 0x08014900, 0x00100,  329},
  { 0x08014A00, 0x00100,  330},
  { 0x08014B00, 0x00100,  331},
  { 0x08014C00, 0x00100,  332},
  { 0x08014D00, 0x00100,  333},
  { 0x08014E00, 0x00100,  334},
  { 0x08014F00, 0x00100,  335},
  { 0x08015000, 0x00100,  336},
  { 0x08015100, 0x00100,  337},
  { 0x08015200, 0x00100,  338},
  { 0x08015300, 0x00100,  339},
  { 0x08015400, 0x00100,  340},
  { 0x08015500, 0x00100,  341},
  { 0x08015600, 0x00100,  342},
  { 0x08015700, 0x00100,  343},
  { 0x08015800, 0x00100,  344},
  { 0x08015900, 0x00100,  345},
  { 0x08015A00, 0x00100,  346},
  { 0x08015B00, 0x00100,  347},
  { 0x08015C00, 0x00100,  348},
  { 0x08015D00, 0x00100,  349},
  { 0x08015E00, 0x00100,  350},
  { 0x08015F00, 0x00100,  351},
  { 0x08016000, 0x00100,  352},
  { 0x08016100, 0x00100,  353},
  { 0x08016200, 0x00100,  354},
  { 0x08016300, 0x00100,  355},
  { 0x08016400, 0x00100,  356},
  { 0x08016500, 0x00100,  357},
  { 0x08016600, 0x00100,  358},
  { 0x08016700, 0x00100,  359},
  { 0x08016800, 0x00100,  360},
  { 0x08016900, 0x00100,  361},
  { 0x08016A00, 0x00100,  362},
  { 0x08016B00, 0x00100,  363},
  { 0x08016C00, 0x00100,  364},
  { 0x08016D00, 0x00100,  365},
  { 0x08016E00, 0x00100,  366},
  { 0x08016F00, 0x00100,  367},
  { 0x08017000, 0x00100,  368},
  { 0x08017100, 0x00100,  369},
  { 0x08017200, 0x00100,  370},
  { 0x08017300, 0x00100,  371},
  { 0x08017400, 0x00100,  372},
  { 0x08017500, 0x00100,  373},
  { 0x08017600, 0x00100,  374},
  { 0x08017700, 0x00100,  375},
  { 0x08017800, 0x00100,  376},
  { 0x08017900, 0x00100,  377},
  { 0x08017A00, 0x00100,  378},
  { 0x08017B00, 0x00100,  379},
  { 0x08017C00, 0x00100,  380},
  { 0x08017D00, 0x00100,  381},
  { 0x08017E00, 0x00100,  382},
  { 0x08017F00, 0x00100,  383},
  { 0x08018000, 0x00100,  384},
  { 0x08018100, 0x00100,  385},
  { 0x08018200, 0x00100,  386},
  { 0x08018300, 0x00100,  387},
  { 0x08018400, 0x00100,  388},
  { 0x08018500, 0x00100,  389},
  { 0x08018600, 0x00100,  390},
  { 0x08018700, 0x00100,  391},
  { 0x08018800, 0x00100,  392},
  { 0x08018900, 0x00100,  393},
  { 0x08018A00, 0x00100,  394},
  { 0x08018B00, 0x00100,  395},
  { 0x08018C00, 0x00100,  396},
  { 0x08018D00, 0x00100,  397},
  { 0x08018E00, 0x00100,  398},
  { 0x08018F00, 0x00100,  399},
  { 0x08019000, 0x00100,  400},
  { 0x08019100, 0x00100,  401},
  { 0x08019200, 0x00100,  402},
  { 0x08019300, 0x00100,  403},
  { 0x08019400, 0x00100,  404},
  { 0x08019500, 0x00100,  405},
  { 0x08019600, 0x00100,  406},
  { 0x08019700, 0x00100,  407},
  { 0x08019800, 0x00100,  408},
  { 0x08019900, 0x00100,  409},
  { 0x08019A00, 0x00100,  410},
  { 0x08019B00, 0x00100,  411},
  { 0x08019C00, 0x00100,  412},
  { 0x08019D00, 0x00100,  413},
  { 0x08019E00, 0x00100,  414},
  { 0x08019F00, 0x00100,  415},
  { 0x0801A000, 0x00100,  416},
  { 0x0801A100, 0x00100,  417},
  { 0x0801A200, 0x00100,  418},
  { 0x0801A300, 0x00100,  419},
  { 0x0801A400, 0x00100,  420},
  { 0x0801A500, 0x00100,  421},
  { 0x0801A600, 0x00100,  422},
  { 0x0801A700, 0x00100,  423},
  { 0x0801A800, 0x00100,  424},
  { 0x0801A900, 0x00100,  425},
  { 0x0801AA00, 0x00100,  426},
  { 0x0801AB00, 0x00100,  427},
  { 0x0801AC00, 0x00100,  428},
  { 0x0801AD00, 0x00100,  429},
  { 0x0801AE00, 0x00100,  430},
  { 0x0801AF00, 0x00100,  431},
  { 0x0801B000, 0x00100,  432},
  { 0x0801B100, 0x00100,  433},
  { 0x0801B200, 0x00100,  434},
  { 0x0801B300, 0x00100,  435},
  { 0x0801B400, 0x00100,  436},
  { 0x0801B500, 0x00100,  437},
  { 0x0801B600, 0x00100,  438},
  { 0x0801B700, 0x00100,  439},
  { 0x0801B800, 0x00100,  440},
  { 0x0801B900, 0x00100,  441},
  { 0x0801BA00, 0x00100,  442},
  { 0x0801BB00, 0x00100,  443},
  { 0x0801BC00, 0x00100,  444},
  { 0x0801BD00, 0x00100,  445},
  { 0x0801BE00, 0x00100,  446},
  { 0x0801BF00, 0x00100,  447},
  { 0x0801C000, 0x00100,  448},
  { 0x0801C100, 0x00100,  449},
  { 0x0801C200, 0x00100,  450},
  { 0x0801C300, 0x00100,  451},
  { 0x0801C400, 0x00100,  452},
  { 0x0801C500, 0x00100,  453},
  { 0x0801C600, 0x00100,  454},
  { 0x0801C700, 0x00100,  455},
  { 0x0801C800, 0x00100,  456},
  { 0x0801C900, 0x00100,  457},
  { 0x0801CA00, 0x00100,  458},
  { 0x0801CB00, 0x00100,  459},
  { 0x0801CC00, 0x00100,  460},
  { 0x0801CD00, 0x00100,  461},
  { 0x0801CE00, 0x00100,  462},
  { 0x0801CF00, 0x00100,  463},
  { 0x0801D000, 0x00100,  464},
  { 0x0801D100, 0x00100,  465},
  { 0x0801D200, 0x00100,  466},
  { 0x0801D300, 0x00100,  467},
  { 0x0801D400, 0x00100,  468},
  { 0x0801D500, 0x00100,  469},
  { 0x0801D600, 0x00100,  470},
  { 0x0801D700, 0x00100,  471},
  { 0x0801D800, 0x00100,  472},
  { 0x0801D900, 0x00100,  473},
  { 0x0801DA00, 0x00100,  474},
  { 0x0801DB00, 0x00100,  475},
  { 0x0801DC00, 0x00100,  476},
  { 0x0801DD00, 0x00100,  477},
  { 0x0801DE00, 0x00100,  478},
  { 0x0801DF00, 0x00100,  479},
  { 0x0801E000, 0x00100,  480},
  { 0x0801E100, 0x00100,  481},
  { 0x0801E200, 0x00100,  482},
  { 0x0801E300, 0x00100,  483},
  { 0x0801E400, 0x00100,  484},
  { 0x0801E500, 0x00100,  485},
  { 0x0801E600, 0x00100,  486},
  { 0x0801E700, 0x00100,  487},
  { 0x0801E800, 0x00100,  488},
  { 0x0801E900, 0x00100,  489},
  { 0x0801EA00, 0x00100,  490},
  { 0x0801EB00, 0x00100,  491},
  { 0x0801EC00, 0x00100,  492},
  { 0x0801ED00, 0x00100,  493},
  { 0x0801EE00, 0x00100,  494},
  { 0x0801EF00, 0x00100,  495},
  { 0x0801F000, 0x00100,  496},
  { 0x0801F100, 0x00100,  497},
  { 0x0801F200, 0x00100,  498},
  { 0x0801F300, 0x00100,  499},
  { 0x0801F400, 0x00100,  500},
  { 0x0801F500, 0x00100,  501},
  { 0x0801F600, 0x00100,  502},
  { 0x0801F700, 0x00100,  503},
  { 0x0801F800, 0x00100,  504},
  { 0x0801F900, 0x00100,  505},
  { 0x0801FA00, 0x00100,  506},
  { 0x0801FB00, 0x00100,  507},
  { 0x0801FC00, 0x00100,  508},
  { 0x0801FD00, 0x00100,  509},
  { 0x0801FE00, 0x00100,  510},
  { 0x0801FF00, 0x00100,  511} /* TODO */
  #endif
};
#endif

/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#if 0
/**
  * @brief  This function is used to read data from a given address.
  * @param  Address The address to be read.
  * @retval Returns the read value.
  */
uint16_t FLASHLAYOUT_GetSectorIdx(uint32_t address)
{
  uint16_t result = FLASH_INVALID_SECTOR_IDX;
  uint16_t sectorIdx;

  /* search through the sectors to find the right one */
  for (sectorIdx = 0; sectorIdx < FLASH_TOTAL_SECTORS; sectorIdx++)
  {
    /* is the address in this sector? */
    if ((address >= flashLayout[sectorIdx].sector_start) && \
        (address < (flashLayout[sectorIdx].sector_start + \
                    flashLayout[sectorIdx].sector_size)))
    {
      /* update the result value and stop looping */
      result = sectorIdx;
      break;
    }
  }

  /* give the result back to the caller */
  return result;
}
#else
uint16_t FLASHLAYOUT_GetPageNum(uint32_t address)
{
    return (uint16_t)((address & 0x7FF00) >> 8);
}
#endif

/**
  * @brief  This function is retured page number from a given address.
  * @param  Address The address to be searched.
  * @retval Returns the page value.
  */
uint32_t FLASHLAYOUT_GetPages(uint32_t start_address, uint32_t total_size, uint8_t *RAM_Buf, uint16_t RAM_BUFFER_SIZE)
{
    uint16_t *pages_buf  = (uint16_t *)RAM_Buf;

    uint16_t utotal_pages;
    uint32_t utotal_sizes;

    #if 0
    uint16_t sectorIdx = FLASHLAYOUT_GetSectorIdx(start_address);

    utotal_pages = 0;
    utotal_sizes = 0;

    //total pages ?????? ?????? ??????
    //1. number of pages
    pages_buf++;
    while( (utotal_sizes < total_size) && (utotal_pages < ((RAM_BUFFER_SIZE / 2) - 1)) )
    {
      utotal_sizes += flashLayout[sectorIdx].sector_size;
      *pages_buf = flashLayout[sectorIdx].sector_num;
  
      //2. page number to be  erased
      utotal_pages++;
      sectorIdx++;
      pages_buf++;
    }
    #else
    utotal_pages = 0;
    utotal_sizes = 0;

    uint16_t cur_pagenum = FLASHLAYOUT_GetPageNum(start_address);

    if(cur_pagenum < 128) {
      return 0;
    }

    //total pages ?????? ?????? ??????
    //1. number of pages
    pages_buf++;   
    while( (utotal_sizes < total_size) && (utotal_pages < ((RAM_BUFFER_SIZE / 2) - 1)) )
    {
      utotal_sizes += FLASH_PAGE_SIZE;
      *pages_buf = cur_pagenum++;
  
      //2. page number to be  erased
      utotal_pages++;
      pages_buf++;
    }

    #endif

    pages_buf  = (uint16_t *)RAM_Buf;
    *pages_buf = utotal_pages;

    // return total erased size
    return utotal_sizes;
}

/**
  * @brief  This function convert from page to Start Address.
  * @param  Page The page to be converted.
  * @retval Returns the Start Address value.
  */
uint32_t FLASHLAYOUT_GetStartAddress(uint16_t page_number)
{
  #if 0
  uint16_t sectorIdx;

  /* search through the sectors to find the right one */
  for (sectorIdx = 0; sectorIdx < FLASH_TOTAL_SECTORS; sectorIdx++)
  {
    /* is the address in this sector? */
    if (page_number == flashLayout[sectorIdx].sector_num) 
    {
      return flashLayout[sectorIdx].sector_start;
    }
  }
  return 0;
  #else
  return (FLASH_BASE + page_number *FLASH_PAGE_SIZE);
  #endif
}  

