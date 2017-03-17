#include<cstdio>
#include<iostream>
#include<queue>
#include<vector>
using namespace std;

int GLOBAL_TIME = 0;  // in seconds.
int MAX_SECONDS = 360000;

class Tourist {
public:
    Tourist() {
        this->id = -1;
        this->nextEventTime = -1;
        this->countSites = 0;
        this->totalWaitTime = 0;
    }
    Tourist(const Tourist& a) {
        this->id = a.id;
        this->nextEventTime = a.nextEventTime;
        this->countSites = a.countSites;
        this->totalWaitTime = a.totalWaitTime;
    }
    Tourist(int id, int nextEventTime) {
        this->id = id;
        this->nextEventTime = nextEventTime;
        this->countSites = 0;
        this->totalWaitTime = 0;
    }
    int getNextEventTime() const {return nextEventTime;}
    void setNextEventTime(int nextEventTime) {
        this->nextEventTime = nextEventTime;
    }
    // Define the comparation between two Tourist by their next event time value.
    // The one with smaller time will be arranged in the front of the Containers.
    bool operator < (const Tourist& b) const {
        return this->nextEventTime > b.nextEventTime;
    }
private:
    int id;
    int nextEventTime;

    // Records
    int countSites;
    int totalWaitTime;
};

// Each object of this class is a queue of Tourists.
class Que {
public:
    // Enter the queue at the rear.
    void push(Tourist &a) { q.push(a); }
    // Return the first most element in the queue.
    Tourist front() { return this->q.front(); }
    // Leave the queue at the head.
    void pop() { q.pop(); }
    // if the queue is empty;
    bool empty() { return q.empty(); }
    int getSize() { return q.size(); }
private:
    queue<Tourist> q;
};

// Each object of this class is a container of Tourists.
// Container can output the minimium element in O(1) time.
class Container {
public:
    void addOne(Tourist &a) {
        this->c.push(a);
    }
    Tourist top() { return this->c.top(); }
    void removeTop() { c.pop(); }
    bool empty() { return c.empty(); }
    int getSize() { return c.size(); }
private:
    // The smaller next event time toursits will have higher priority.
    priority_queue<Tourist> c;
};

int random_time() {
    return 1 + (rand() % 10);
}

int main() {
    srand(0);
    time_t startTime, endTime;
    time(&startTime);

    queue<int> arriveTime[4];
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 4; j++) {
            arriveTime[j].push((i + 1) * 10);
        }
    }

    int tourist_id = 0;

    Que in_queue[4], out_queue[4];
    Container sites_container[4];
    Container paths_container[4];

    int docker_num[4], bicycle_num[4];

    for (int i = 0; i < 4; i++) {
        docker_num[i] = bicycle_num[i] = 80;
    }

    /* The picture of A, B, C, D four sites.
     *
     * B                                      A
     *   sites[1]  <-  paths[0]  <-  sites[0]
     *      |                           ^
     *      v                           |
     *   paths[1]                    paths[3]
     *      |                           ^
     *      v                           |
     *   sites[2]  ->  paths[2]  ->  sites[3]
     * C                                      D
     *
     *
     * */

    // Run for 200 seconds.
    while (GLOBAL_TIME <= MAX_SECONDS) {

        // Update 4 sites situation at this second.
        for (int i = 0; i < 4; i++) {

            // The last site of site i.
            int last = (i - 1 >= 0) ? (i - 1) : 3;

            // Check if there are people arrived at site[i] from outside the theme park.
            while (!arriveTime[i].empty()) {
                int timestamp = arriveTime[i].front();
                if (timestamp != GLOBAL_TIME) {
                    break;
                }
                arriveTime[i].pop();
                Tourist t(tourist_id++, GLOBAL_TIME + random_time());
                sites_container[i].addOne(t);
            }

            // Check if there are people arrived at site[i] from the last site.
            while (!paths_container[last].empty()) {
                // Tourist t is the one who will firstly finish paths[i] in the path container.
                Tourist t = paths_container[last].top();
                if (t.getNextEventTime() != GLOBAL_TIME) break;
                paths_container[last].removeTop();
                // The tourist will go into the in queue,
                // so his leaving of queue depends on the num of bicycles at sites[i],
                // rather than a determined period of time. Here we set the next event time to -1.
                t.setNextEventTime(-1);
                // Tourist t enters in the in queue of the next site.
                in_queue[i].push(t);
            }

            // Check if the in queue is emtpy and if there is empty docker.
            while (!in_queue[i].empty() && bicycle_num[i] < docker_num[i]) {
                // Tourist t is at the head of the in queue.
                Tourist t = in_queue[i].front();
                in_queue[i].pop();
                // Tourist t return a bicycle.
                bicycle_num[i]++;
                // Set how long will the tourist play in sites[i].
                t.setNextEventTime(GLOBAL_TIME + random_time());
                // Let Tourist t into sites[i].
                sites_container[i].addOne(t);
            }

            // Check if there are people want to out among the current tourists in sites[i].
            while (!sites_container[i].empty()) {
                // Tourist t is the one who will firstly leave sits[i] in the site container.
                Tourist t = sites_container[i].top();
                if (t.getNextEventTime() != GLOBAL_TIME) break;
                sites_container[i].removeTop();
                // The tourist will go to a queue,
                // so his leaving of queue depends on the num of empty dockers,
                // rather than a determined period of time. Here we set the next event time to -1.
                t.setNextEventTime(-1);
                // Tourist t enters in the out queue.
                out_queue[i].push(t);
            }

            // Check if the out queue is emtpy and if there is bicycle left.
            while (!out_queue[i].empty() && bicycle_num[i] > 0) {
                // Tourist t is at the head of the out queue.
                Tourist t = out_queue[i].front();
                out_queue[i].pop();
                // Tourist t return a bicycle.
                bicycle_num[i]--;
                // Set how long will the tourist spend on paths[i].
                int temp = random_time();
                temp += GLOBAL_TIME;
                t.setNextEventTime(temp);
                // Tourist t on his way to the next site.
                paths_container[i].addOne(t);
            }
        }

        // Output the status of this second.
        cout << "In second " << GLOBAL_TIME;
        for (int i = 0; i < 4; i++) {
            cout << "  " << char(i + 'A') << ": " << sites_container[i].getSize()
                 << "  In " << char(i + 'A') << ": " << in_queue[i].getSize()
                 << "  Out " << char(i + 'A') << ": " << out_queue[i].getSize();
        }
        cout << endl;
        GLOBAL_TIME++;
    }

    time(&endTime);

    cout << "Similated " << MAX_SECONDS / (24 * 3600) << " days.  "
         << "Cost " << int(endTime - startTime) << " seconds.";

    // Calculate the average waiting time.
    // ...
    return 0;
}
