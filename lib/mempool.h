#ifndef MEM_POOL
#define MEM_POOL


#include <stddef.h>


/* 对齐到指针大小的倍数 */
#define ALIGN(n)      (((n) + sizeof(void*) - 1) & ~(sizeof(void*) - 1))
/* 每次扩容新段的最小大小 */
#define EXPAND_MIN    4096
/* 从用户指针回退到 BlockHeader */
#define HDR(ptr)      ((BlockHeader *)((char *)(ptr) - sizeof(BlockHeader)))
/* 一次完整分配需要的总字节：header + 数据区 */
#define TOTAL(sz)     (sizeof(BlockHeader) + (sz))


/*
 * 块头：紧贴在每块分配数据的前面，记录本块数据区的对齐后大小。
 * 用户拿到的指针指向 header 之后；mp_free / mp_realloc 向前偏移
 * sizeof(BlockHeader) 读回大小，无需调用方手动传 size。
 */
typedef struct BlockHeader {
  size_t size;   /* 数据区对齐后大小（不含 header 本身） */
} BlockHeader;

/* 空闲链表节点（直接复用已释放块的内存空间） */
typedef struct FreeBlock {
  struct FreeBlock *next;  /* 方便地插入删除 */
  size_t           size;   /* 数据区大小，含义同 BlockHeader.size */
} FreeBlock;

/* 内存池段；多段通过 next 链接，实现自动扩容 */
typedef struct MemPool {
  void           *buffer;    /* 线性缓冲区起始地址 */
  size_t          buf_size;  /* 缓冲区总大小 */
  size_t          used;      /* 已使用字节数（header + 数据 + 对齐） */
  FreeBlock       *free_list; /* 已释放块的单链表 */
  struct MemPool *next;      /* 扩容后追加的下一段 */
} MemPool;


static MemPool *_mp_create (size_t initial_size);
static void    *_mp_malloc (MemPool *pool, size_t size);
static void    *_mp_calloc (MemPool *pool, size_t nmemb, size_t size);
static void    *_mp_realloc(MemPool *pool, void *ptr, size_t new_size);
static void     _mp_free   (MemPool *pool, void *ptr);
static void     _mp_destroy(MemPool *pool);


static MemPool* global_pool;

/* ------------------------------------------------------------------ */
/* 内部：创建一个新的池段                                               */
/* ------------------------------------------------------------------ */
static MemPool *
_new_segment (size_t size)
{
  MemPool *seg = (MemPool *)malloc(sizeof(MemPool));
  if (!seg) { return NULL; }
  seg->buffer = malloc(size);
  if (!seg->buffer) { free(seg); return NULL; }
  seg->buf_size  = size;
  seg->used      = 0;
  seg->free_list = NULL;
  seg->next      = NULL;
  return seg;
}

/* ------------------------------------------------------------------ */
/* 内部：核心分配（返回数据区指针，已写好 BlockHeader）                  */
/* ------------------------------------------------------------------ */
static void *
_do_alloc (MemPool *pool, size_t size)
{
  /* size 已由调用方对齐 */
  size_t total = TOTAL(size);

  /* 1. 空闲链表复用 */
  MemPool *seg = pool;
  while (seg)
    {
      FreeBlock **prev = &seg->free_list;
      FreeBlock  *blk  =  seg->free_list;
      while (blk)
        {
          if (blk->size >= size)
            {
              *prev = blk->next;
              /* 写入 header，返回数据区 */
              BlockHeader *hdr = (BlockHeader *)blk;
              hdr->size = size;
              return (char *)hdr + sizeof(BlockHeader);
            }
          prev = &blk->next;  /* &blk->next == blk->prev because: 1 of struct is next */
          blk  =  blk->next;
        }
      seg = seg->next;
    }

  /* 2. 线性区顺序分配 */
  MemPool *last = pool;
  seg = pool;
  while (seg)
    {
      if (seg->used + total <= seg->buf_size) {
        BlockHeader *hdr = (BlockHeader *)((char *)seg->buffer + seg->used);
        hdr->size  = size;
        seg->used += total;
        return (char *)hdr + sizeof(BlockHeader);
      }
      last = seg;
      seg  = seg->next;
    }

  /* 3. 扩容：追加新段 */
  size_t new_size  = (total > EXPAND_MIN) ? total * 2 : EXPAND_MIN;
  MemPool *new_seg = _new_segment(new_size);
  if (!new_seg) { return NULL; }
  last->next = new_seg;

  BlockHeader *hdr = (BlockHeader *)new_seg->buffer;
  hdr->size     = size;
  new_seg->used = total;
  return (char *)hdr + sizeof(BlockHeader);
}

/**
 * @brief 创建一个新的内存池
 *
 * @param initial_size 初始分配的大小（字节）。如果小于内部最小阈值，将自动调整为最小值。
 *
 * @return 成功返回指向新内存池的指针，失败返回 NULL（通常在内存不足时）。
 */
MemPool *
_mp_create (size_t initial_size)
{
  if (initial_size < EXPAND_MIN) initial_size = EXPAND_MIN;

  return _new_segment(initial_size);
}

/**
 * @brief 从内存池中分配指定大小的内存块
 *
 * @param pool 指向内存池的指针
 * @param size 需要分配的字节数
 *
 * @return 成功返回指向已分配内存的指针，失败返回 NULL（如 pool 为空、size 为 0 或内存不足）。
 *         返回的内存未初始化。
 */
void *
_mp_malloc (MemPool *pool, size_t size)
{
  if (!pool || size == 0) { return NULL; }

  return _do_alloc(pool, ALIGN(size));
}

/**
 * @brief 从内存池中分配并清零指定大小的内存块
 *
 * @param pool 指向内存池的指针
 * @param nmemb 元素个数
 * @param size 每个元素的字节数
 *
 * @return 成功返回指向已分配且清零内存的指针，失败返回 NULL。
 *         若发生整数溢出（nmemb * size 超出范围）也会返回 NULL。
 */
void *
_mp_calloc (MemPool *pool, size_t nmemb, size_t size)
{
  if (!pool || nmemb == 0 || size == 0) { return NULL; }
  /* 溢出检查 */
  if (size != 0 && nmemb > (size_t)-1 / size) { return NULL; }

  size_t total = nmemb * size;
  void *ptr = _do_alloc(pool, ALIGN(total));
  if (ptr) memset(ptr, 0, total);
  return ptr;
}

/**
 * @brief 调整内存块的大小
 *
 * @param pool    指向内存池的指针
 * @param ptr     指向之前分配的内存块的指针。若为 NULL，则行为等同于 mp_alloc。
 * @param new_size 新的请求大小（字节）。若为 0，则行为等同于 mp_free 并返回 NULL。
 *
 * @return 成功返回指向新内存块的指针（可能已移动），失败返回 NULL。
 *         若缩小或大小不变，通常原地返回；若扩大，则分配新块、拷贝数据并释放旧块。
 */
void *
_mp_realloc (MemPool *pool, void *ptr, size_t new_size)
{
  /* realloc(pool, NULL, n) → alloc */
  if (!ptr) { return _mp_malloc(pool, new_size); }
  /* realloc(pool, ptr, 0) → free */
  if (new_size == 0) { _mp_free(pool, ptr); return NULL; }

  size_t aligned_new = ALIGN(new_size);
  size_t old_size    = HDR(ptr)->size;   /* 读取块头记录的旧大小 */

  /* 缩小或等大：原地返回，更新 header */
  if (aligned_new <= old_size)
    {
      HDR(ptr)->size = aligned_new;
      return ptr;
    }

  /* 扩大：重新分配 + 拷贝 + 释放旧块 */
  void *new_ptr = _do_alloc(pool, aligned_new);
  if (!new_ptr) { return NULL; }
  memcpy(new_ptr, ptr, old_size);
  _mp_free(pool, ptr);
  return new_ptr;
}

/**
 * @brief 释放之前分配的内存块回内存池
 *
 * @param pool 指向内存池的指针
 * @param ptr  指向要释放的内存块的指针。若为 NULL 或 pool 为 NULL，则直接返回。
 *
 * @note 该操作并非真正归还给操作系统，而是将块标记为空闲并加入空闲链表，供后续分配复用。
 */
void
_mp_free (MemPool *pool, void *ptr)
{
  if (!pool || !ptr) return;
  BlockHeader *hdr = HDR(ptr);          /* 从块头读出大小 */
  FreeBlock    *blk = (FreeBlock *)hdr;   /* 复用同一块内存做链表节点 */
  blk->size        = hdr->size;
  blk->next        = pool->free_list;   /* 头插法插入free_list */
  pool->free_list  = blk;
}

/**
 * @brief 销毁内存池并释放所有相关资源
 *
 * @param pool 指向要销毁的内存池的指针
 *
 * @note 此函数会释放内存池管理的所有内存段（buffer）以及池结构体本身。
 *       调用后不应再使用该 pool 指针或其曾分配的任何指针。
 */
void
_mp_destroy (MemPool *pool)
{
  MemPool *seg = pool;
  while (seg)
    {
      MemPool *next = seg->next;
      free(seg->buffer);
      free(seg);
      seg = next;
    }
}

/**
 * @brief 初始化内存池
 *
 * @param size 初始化内存池的大小
 *
 * @note 此函数在程序开始时调用，请不要在其他时刻调用
 */

__attribute__((constructor))
void
mp_initialize (size_t size)
{
  MemPool *pool = _mp_create(size);
  global_pool = pool;
}

/**
 * @brief 销毁内存池并释放所有相关资源
 *
 * @param None
 *
 * @note 此函数在程序结束时调用，请不要在其他时刻调用
 */
__attribute__((destructor))
void
mp_finalize (void)
{
  _mp_destroy(global_pool);
}


/* ------------------------------------------------------------------ */
/* API                                                                */
/* ------------------------------------------------------------------ */
void *
mp_malloc (size_t size)
{
  return _mp_malloc(global_pool,size);
}

void *
mp_calloc (size_t nmemb, size_t size)
{
 return _mp_calloc(global_pool,nmemb,size);
}

void *
mp_realloc (void *ptr, size_t new_size)
{
  return _mp_realloc(global_pool,ptr,new_size);
}

void
mp_free (void *ptr)
{
  _mp_free(global_pool,ptr);
}

#endif
