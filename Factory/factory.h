#ifndef _factory_h
#define _factory_h

void* parse_data(const char* file_name);
void draw_scene(void* _queue);
void delete_data(void* _queue);


#endif