/*
 * HALIYAL BUS TRACKER — Circular Queue Backend (C Language)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SEATS    20
#define MAX_NAME_LEN 40
#define BUS_ID_LEN   14
#define ROUTE_LEN    32
#define NUM_BUSES     5

typedef struct {
    int  seat_num;
    char name[MAX_NAME_LEN];
    char boarded_at[20];
    int  is_occupied;
} Passenger;

typedef struct {
    Passenger seats[MAX_SEATS];
    int front, rear, size, capacity;
} PassengerQueue;

typedef struct {
    char bus_id[BUS_ID_LEN];
    char from[ROUTE_LEN];
    char to[ROUTE_LEN];
    char departure[12];
    float distance_km;
    float fare_rs;
    PassengerQueue pq;
} Bus;

Bus fleet[NUM_BUSES];

void pq_init(PassengerQueue *q, int cap) {
    q->front=0; q->rear=-1; q->size=0; q->capacity=cap;
    for(int i=0;i<MAX_SEATS;i++) q->seats[i].is_occupied=0;
}
int pq_full(const PassengerQueue *q)  { return q->size==q->capacity; }
int pq_empty(const PassengerQueue *q) { return q->size==0; }

int pq_enqueue(PassengerQueue *q, const char *name) {
    if(pq_full(q)) return -1;
    /* find first free physical slot */
    int slot=-1;
    for(int i=0;i<q->capacity;i++) if(!q->seats[i].is_occupied){slot=i;break;}
    if(slot<0) return -1;
    Passenger *p=&q->seats[slot];
    strncpy(p->name,name,MAX_NAME_LEN-1);
    p->name[MAX_NAME_LEN-1]='\0';
    p->seat_num=slot+1;
    p->is_occupied=1;
    time_t now=time(NULL); struct tm *t=localtime(&now);
    snprintf(p->boarded_at,sizeof(p->boarded_at),"%02d:%02d:%02d",t->tm_hour,t->tm_min,t->tm_sec);
    q->size++;
    return p->seat_num;
}

int pq_dequeue(PassengerQueue *q, Passenger *out) {
    if(pq_empty(q)) return -1;
    for(int i=0;i<q->capacity;i++){
        if(q->seats[i].is_occupied){*out=q->seats[i];q->seats[i].is_occupied=0;q->size--;return 0;}
    }
    return -1;
}

int pq_remove_by_name(PassengerQueue *q, const char *name) {
    for(int i=0;i<q->capacity;i++){
        if(q->seats[i].is_occupied && strcmp(q->seats[i].name,name)==0){
            q->seats[i].is_occupied=0; q->size--; return i+1;
        }
    }
    return -1;
}

int pq_remove_by_seat(PassengerQueue *q, int seat) {
    int idx=seat-1;
    if(idx<0||idx>=q->capacity||!q->seats[idx].is_occupied) return -1;
    q->seats[idx].is_occupied=0; q->size--; return seat;
}

void init_fleet(void) {
    typedef struct {const char *id,*to,*dep; float dist,fare;} R;
    R r[5]={
        {"KA-25-F-101","Dharwad","06:30 AM",98.5f,120.0f},
        {"KA-25-F-202","Dandeli","07:15 AM",24.0f, 35.0f},
        {"KA-25-F-303","Belgaum","08:00 AM",148.3f,180.0f},
        {"KA-25-F-404","Hubli",  "09:00 AM",120.0f,140.0f},
        {"KA-25-F-505","Ramnagar","10:30 AM",312.0f,380.0f}
    };
    for(int i=0;i<NUM_BUSES;i++){
        strncpy(fleet[i].bus_id,r[i].id,BUS_ID_LEN);
        strncpy(fleet[i].from,"Haliyal",ROUTE_LEN);
        strncpy(fleet[i].to,r[i].to,ROUTE_LEN);
        strncpy(fleet[i].departure,r[i].dep,12);
        fleet[i].distance_km=r[i].dist;
        fleet[i].fare_rs=r[i].fare;
        pq_init(&fleet[i].pq,MAX_SEATS);
    }
    printf("[FLEET] %d buses initialized from Haliyal.\n",NUM_BUSES);
}

void display_fleet(void) {
    printf("\n+---+----------------+--------------+-----------+----------+--------+\n");
    printf("| # |   Bus ID       | Destination  | Departure | Distance | Seats  |\n");
    printf("+---+----------------+--------------+-----------+----------+--------+\n");
    for(int i=0;i<NUM_BUSES;i++)
        printf("| %d | %-14s | %-12s | %-9s | %6.1fkm | %2d/%-3d |\n",
               i+1,fleet[i].bus_id,fleet[i].to,fleet[i].departure,
               fleet[i].distance_km,fleet[i].pq.size,fleet[i].pq.capacity);
    printf("+---+----------------+--------------+-----------+----------+--------+\n\n");
}

void display_bus_passengers(int idx){
    Bus *b=&fleet[idx];
    printf("\nBus: %s | %s -> %s | Seats: %d/%d\n",
           b->bus_id,b->from,b->to,b->pq.size,b->pq.capacity);
    printf("+------+----------------------------+----------+\n");
    printf("| Seat | Passenger Name             | Boarded  |\n");
    printf("+------+----------------------------+----------+\n");
    int any=0;
    for(int i=0;i<b->pq.capacity;i++){
        if(b->pq.seats[i].is_occupied){
            printf("|  %2d  | %-26s | %s |\n",
                   b->pq.seats[i].seat_num,b->pq.seats[i].name,b->pq.seats[i].boarded_at);
            any=1;
        }
    }
    if(!any) printf("|      (no passengers yet)                      |\n");
    printf("+------+----------------------------+----------+\n\n");
}

int main(void){
    init_fleet();
    display_fleet();

    printf("=== Boarding Demo ===\n");
    int s;
    s=pq_enqueue(&fleet[0].pq,"Ramesh Kumar");   printf("[BUS-1] Ramesh Kumar -> Seat %d\n",s);
    s=pq_enqueue(&fleet[0].pq,"Sunita Patil");   printf("[BUS-1] Sunita Patil -> Seat %d\n",s);
    s=pq_enqueue(&fleet[0].pq,"Vijay Naik");     printf("[BUS-1] Vijay Naik   -> Seat %d\n",s);
    s=pq_enqueue(&fleet[1].pq,"Anita Desai");    printf("[BUS-2] Anita Desai  -> Seat %d\n",s);
    s=pq_enqueue(&fleet[3].pq,"Kaveri Reddy");   printf("[BUS-4] Kaveri Reddy -> Seat %d\n",s);

    display_bus_passengers(0);

    printf("=== Remove Sunita Patil from Bus-1 ===\n");
    int freed=pq_remove_by_name(&fleet[0].pq,"Sunita Patil");
    if(freed>0) printf("Seat %d freed.\n\n",freed);

    s=pq_enqueue(&fleet[0].pq,"Priya Hegde");
    printf("[BUS-1] Priya Hegde -> Seat %d (reused)\n\n",s);
    display_bus_passengers(0);

    Passenger al;
    pq_dequeue(&fleet[0].pq,&al);
    printf("=== %s (Seat %d) alighted ===\n\n",al.name,al.seat_num);
    display_fleet();
    return 0;
}
