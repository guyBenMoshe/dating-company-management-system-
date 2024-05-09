#pragma once
#define _CRTDBG_MAP_ALLOC

/////Print Macros
#define PRINT_RETURN_ZERO(msg){puts(msg); return 0;}
#define PRINT_RETURN(msg){puts(msg);return;}
#define PRINT_RETURN_NULL(msg){puts(msg);return NULL;}

/////Free Macros
#define FREE_RETURN_ZERO(ptr){free(ptr);return 0;}
#define FREE_PRINT_RETURN_NULL(ptr,msg){free(ptr);PRINT_RETURN_NULL(msg)}

/////File Macros
#define CLOSE_RETURN_ZERO(fp){fclose(fp);return 0;}
#define CLOSE_RETURN_ONE(fp){fclose(fp);return 1;}

/////check Macros
#define CHECK_PTR_PRINT_BREACK(ptr,msg){if(!ptr){puts(msg);break;}}
#define CHECK_PTR_PRINT_RETURN(ptr,msg){if(!ptr){puts(msg);return;}}
#define CHECK_PTR_FREE_RETURN_ZERO(ptrCheck,ptrFree){if(!ptrCheck)FREE_RETURN_ZERO(ptrFree);}
#define CHECK_PTR_RETURN_ZERO(ptr){if(!ptr)return 0;}
#define CHECK_PTR_PRINT_RETURN_NULL(ptr,msg){if(!ptr)PRINT_RETURN_NULL(msg);}
#define	CHECK_CLOSE_RETURN_ZERO(ptr,fp){if(!ptr)CLOSE_RETURN_ZERO(fp)}


