#include <stdio.h>  // for printf()
#include <conio.h>  // for _getch()
#include <stdlib.h> // for rand()

#include <vector>

int obj_id = 0;

class RoadUser
{
public:
    RoadUser()
    {
        relx = -50 + rand() % 101;
        rely = -50 + rand() % 101;
        id = ++obj_id;
    }

    virtual ~RoadUser() {};

    virtual void show_where_you_are()
    {
    }

protected:
    int    id;
    double relx;
    double rely;
    double speed_absolute;
};

class Pedestrian : public RoadUser
{
public:

    Pedestrian()
    {
        printf("\tNew pedestrian detected in our perception field! (id: %d)\n", id);
        speed_absolute = rand() % 5 + 1;
    }

    ~Pedestrian()
    {
        printf("\tPedestrian (id %d) vanished from our perception field!\n", id);
    }

    void show_where_you_are()
    {
        printf("\t\tPedestrian (id %d) at (relx=%.2f,%.2f) with speed %.2f km/h\n", id, relx, rely, speed_absolute);
    }
};

class Car : public RoadUser
{
public:

    Car()
    {
        printf("\tNew car detected in our perception field! (id: %d)\n", id);
        speed_absolute = rand() % 150 + 1;
    }

    ~Car()
    {
        printf("\tCar (id %d) vanished from our perception field!\n", id);
    }

    void show_where_you_are()
    {
        printf("\t\tCar (id %d) at (relx=%.2f,%.2f) with speed %.2f km/h\n", id, relx, rely, speed_absolute);
    }
};


int main()
{
    printf("ObjectListSimulator\n");

    std::vector<RoadUser*> list_all_objects;

    int sim_step = 0;
    while (true)
    {
        printf("\n\nSimulation step: %d\n", ++sim_step);

        // 1. get number N of currently detected road users
        int N = (int)list_all_objects.size();

        // 2. generate a new object or delete one?
        int rnd_number = rand() % 2; // will be 0 or 1
        if ((rnd_number == 0) || (N == 0))
        {
            // generate new road user

            // pedestrian or car?
            int rnd_number_objtype = rand() % 2;
            RoadUser* r;
            switch (rnd_number_objtype)
            {
                case 0: r = new Pedestrian(); break;
                case 1: r = new Car();        break;
            }
            list_all_objects.push_back(r);
        }
        else
        {
            // delete road user 

            int rnd_idx = rand() % N;
            RoadUser* r = list_all_objects[rnd_idx];
            list_all_objects.erase(list_all_objects.begin() + rnd_idx);
            delete r;
        }

        // 3. show all objects currently in list
        printf("\tList of all road users:\n");
        for (unsigned int i = 0; i < list_all_objects.size(); i++)
        {
            RoadUser* r = list_all_objects.at(i);
            r->show_where_you_are();
        }

        // 4. wait for a key press
        printf("Press a key to continue.\n");
        _getwch();

    } // while

} // main()

