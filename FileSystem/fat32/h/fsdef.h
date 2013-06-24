
#define SEEK_SET           -1
#define SEEK_CUR            0
#define SEEK_END            1

typedef struct
{
LONG CurrentOffset;     
LONG CurrentCluster;
}HANDLECONTEXT;
