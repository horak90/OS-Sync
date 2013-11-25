#include <stdio.h>
#include "reader_writer.h"
#include "reader_writer_tracing.h"


extern tracing_t t; 

typedef struct reader_writer
{
  pthread_mutex_t lock;    
  pthread_cond_t condition;
} reader_writer_s; 

reader_writer_t rw_init()
{
  reader_writer_t rw = malloc(sizeof(reader_writer_s)); 

  if(pthread_mutex_init(&(rw->lock), NULL) != 0) 
  {
    perror("pthread_mutex_init"); 
    exit(1);   
  }

  if(pthread_cond_init(&(rw->condition), NULL) != 0) 
  {
    perror("pthread_cond_init");
    exit(1);
  }
  
  return rw; 
}

void begin_read(reader_writer_t rw)
{
  pthread_mutex_lock(&(rw->lock));
  tracing_record_event(t, BR_EVENT_ID);
}

void end_read(reader_writer_t rw)
{
  tracing_record_event(t, ER_EVENT_ID);  
  pthread_mutex_unlock(&(rw->lock));
}

void begin_write(reader_writer_t rw)
{
  pthread_mutex_lock(&(rw->lock));  
  //pthread_cond_wait(&(rw->condition), &(rw->lock));
  tracing_record_event(t, BW_EVENT_ID);  
}

void end_write(reader_writer_t rw)
{
  tracing_record_event(t, EW_EVENT_ID);    
  //pthread_cond_signal(&(rw->condition));
  pthread_mutex_unlock(&(rw->lock));
}

