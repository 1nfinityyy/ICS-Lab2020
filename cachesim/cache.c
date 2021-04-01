#include "common.h"
#include <inttypes.h>
#include<assert.h>
void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);
static uint64_t cycle_cnt = 0;
static uint64_t total_size;//cache size 2^
static uint64_t associativity_size;//one group has 2^
uint64_t group=0;//总的组数
uint64_t cacheline=0;//cache行数
void cycle_increase(int n) { cycle_cnt += n; }


typedef struct 
{
  bool valid;
  bool change;
  uint32_t tag;
  uint32_t data[16]; //data 必须是32位,因为difftest的时候是32位
}Cache;

static Cache *cache;
void write_back(uint32_t group_number,uint32_t lines){
  cache[lines].valid=false;
  if(cache[lines].change==1){
  uintptr_t mem_addr=(cache[lines].tag<<group)+group_number;
  mem_write(mem_addr,(uint8_t*)(cache[lines].data));
  cache[lines].change=0;
  }
}

// TODO: implement the following functions
int line_choose(uintptr_t addr,uint32_t group_number,uint32_t tag){
    int every_group=exp2(associativity_size);
    int start=group_number*every_group;
    int end=(group_number+1)*every_group;
    assert(start>=0&&end>=0);
      for(int i=start;i<end;i++){//需要<不然会找到下一组
      if(cache[i].valid==false){
        uintptr_t mem_addr=(addr>>BLOCK_WIDTH);
        // printf("%lx",mem_addr);
        mem_read(mem_addr,(uint8_t*)(cache[i].data));
        cache[i].valid=true;
        cache[i].change=false;
        cache[i].tag=tag;
        return i;
      }
  }
//如果都是满的.就随机替换
    int random=rand()%every_group;
    int replace=start+random;
    assert(replace<=end);
    write_back(group_number,replace);
    //回写
    uintptr_t  mem_addr=(addr>>BLOCK_WIDTH);
    mem_read(mem_addr,(uint8_t*)(cache[replace].data));
    cache[replace].valid=true;
    cache[replace].change=false;
    cache[replace].tag=tag;
    return replace;
}

int find(uint32_t group_number,uint32_t tag){
  int every_group=exp2(associativity_size);
  int start=group_number*every_group;
  int end=(group_number+1)*(every_group);
  for(int i=start;i<end;i++){//需要<不然会找到下一组
      if(cache[i].tag==tag&&cache[i].valid==1)  return i;
  }
  return -1;
}


uint32_t cache_read(uintptr_t addr) {
  // printf("read %lx \n",addr);
    // if(addr==0x1fbd23a ) printf("reaf 8\n");
  uint32_t tag=addr>>(group+BLOCK_WIDTH);//tag
  // printf("%x\n",tag);
  //get group number
  uint32_t temp=(1<<group)-1;
  uint32_t group_num=(addr>>BLOCK_WIDTH)&temp;//组号
  // printf("%d\n",group_num);
  uint32_t group_addr=(addr&0x3f)>>2;//组内地址
  // printf("%x\n",group_addr);
  int line_number=find(group_num,tag); //行号
  // printf("line is %d\n",line_number);
  if(line_number!=-1){
    // printf("has\n");
      return cache[line_number].data[group_addr];
  }
  else{//去主存找,并且写回去
    int line=line_choose(addr,group_num,tag);
    // printf("%x\n",cache[line].data[group_addr]);
    return cache[line].data[group_addr];
  }
  return 0;
}

// 往 cache 中 addr 地址所属的块写入数据 data，写掩码为 wmask
// 例如当 wmask 为 0xff 时，只写入低8比特
// 若缺失，需要从先内存中读入数据
void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  // if(addr==0x1fbd23a ) printf("write 8\n %x",wmask);
  // printf("write %lx\n",addr);
  uint32_t tag=addr>>(group+BLOCK_WIDTH);//tag
  //get group number
  uint32_t temp=(1<<group)-1;
  uint32_t group_num=(addr>>BLOCK_WIDTH)&temp;//组号
  uint32_t group_addr=(addr&0x3f)>>2;//组内地址 32wei 
  // if(addr==0x1fbd23a ) printf("\n%d %d \n",group_num,group_addr);
  int line_number=-1; //行号
  int every_group=exp2(associativity_size);
  int start=group_num*every_group;
  int end=(group_num+1)*(every_group);
  for(int i=start;i<end;i++){//需要<不然会找到下一组
    if(cache[i].tag==tag){
      line_number=i;
      }
  }
  if(line_number!=-1){//has data
    cache[line_number].change=1;
    cache[line_number].data[group_addr] &= (~wmask);//make other data
	  cache[line_number].data[group_addr] |= (data & wmask);
    // cache[line_number].data[group_addr] = (data & wmask);
  }
  else{//write new
    int new_line=line_choose(addr,group_num,tag);
    // if(addr==0x1fbd23a ) printf("new is %d\n",new_line);
    cache[new_line].change=1;
    cache[new_line].data[group_addr] &= (~wmask);
	  cache[new_line].data[group_addr] |= (data & wmask);
    // cache[line_number].data[group_addr] = (data & wmask);
  }
}

// 初始化一个数据大小为 2^total_size_width B，关联度为 2^associativity_width 的 cache
// 例如 init_cache(14, 2) 将初始化一个 16KB，4 路组相联的cache
// 将所有 valid bit 置为无效即可
void init_cache(int total_size_width, int associativity_width) {
  total_size=total_size_width;
  associativity_size=associativity_width;
  assert(total_size_width > associativity_width);
  group=total_size_width-BLOCK_WIDTH-associativity_width;
  cacheline=exp2(total_size_width-BLOCK_WIDTH);
  cache = (Cache*)malloc(sizeof(Cache) * cacheline);
  for(int i=0;i<cacheline;i++){
    cache[i].valid=false;
    cache[i].change=false;
  }
}

void display_statistic(void) {
}
