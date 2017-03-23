#include<cstdio>
#include<iostream>
#include<queue>
#include<vector>
#include <sys/time.h>
#include <random>
#include <fstream>

using namespace std;

#define NUM_DOCKER 80
#define NUM_BICYCLE 80
#define MAX_SECONDS 3600000 * 2
//const double DISTANCE[4] = {3, 3.5, 4, 2};

int GLOBAL_TIME = 0;  // in seconds.

class Tourist {
public:
    Tourist() {
        this->id = -1;
        this->nextEventTime = -1;
        this->enterQueTime = -1;
        this->countSites = 0;
        this->totalWaitTime = 0;
        this->arrivalTime = 0;
        this->leaveTime = 0;
        this->inWaitTime.resize(4, 0);
        this->outWaitTime.resize(4, 0);
        this->playTime.resize(4, 0);
        this->pathTime.resize(4, 0);
    }
    Tourist(const Tourist& a) {
        this->id = a.id;
        this->nextEventTime = a.nextEventTime;
        this->enterQueTime = -1;
        this->countSites = a.countSites;
        this->totalWaitTime = a.totalWaitTime;
        this->arrivalTime = 0;
        this->leaveTime = 0;
        this->inWaitTime.resize(4, 0);
        this->outWaitTime.resize(4, 0);
        this->playTime.resize(4, 0);
        this->pathTime.resize(4, 0);
    }
    Tourist(int id, int nextEventTime) {
        this->id = id;
        this->nextEventTime = nextEventTime;
        this->enterQueTime = -1;
        this->countSites = 0;
        this->totalWaitTime = 0;
        this->arrivalTime = 0;
        this->leaveTime = 0;
        this->inWaitTime.resize(4, 0);
        this->outWaitTime.resize(4, 0);
        this->playTime.resize(4, 0);
        this->pathTime.resize(4, 0);
    }
    int getNextEventTime() const {return this->nextEventTime;}
    void setNextEventTime(int nextEventTime) {
        this->nextEventTime = nextEventTime;
    }
    void setCountSites(int a) { this->countSites = a; }
    int getCountSites() const { return this->countSites; }
    double getAveWaitTime() {
        if (countSites == 0) return -1;
        return (double)totalWaitTime / 6;
    }
    int getTotalWaitTime() const { return this->totalWaitTime; }
    void setTotalWaitTime(int a) { this->totalWaitTime = a; }
    int getEnterQueTime() const { return this->enterQueTime; }
    void setEnterQueTime(int a) { this->enterQueTime = a; }
    void setInWaitTime(int index, int a) {this->inWaitTime[index] = a;}
    int getInWaitTime(int index) {return this->inWaitTime[index];}
    void setOutWaitTime(int index, int a) {this->outWaitTime[index] = a;}
    int getOutWaitTime(int index) {return this->outWaitTime[index];}
    void setArriveTime(int a) {this->arrivalTime = a;}
    void setLeaveTime(int a) {this->leaveTime = a;}
    void setPlayTime(int siteIndex, int a) {this->playTime[siteIndex] = a;}
    void setPathTime(int siteIndex, int a) {this->pathTime[siteIndex] = a;}
    void setEnterConTime(int a) {this->enterConTime = a;}
    int getEnterConTime() {return this->enterConTime;}
    void appendRoute(int a) {this->route.push_back(a);}
    string displayRoute() {
        char temp[500];
        if (leaveTime == 0) {
            sprintf(temp, "Tourist Id %d arrive at %d: ", this->id, this->arrivalTime);
            string ret(temp);
            return ret + string("Not complete route yet.\n");
        }
        string ret;
        sprintf(temp, "Tourist Id %d arrive at %d: ", this->id, this->arrivalTime);
        ret += string(temp);
        for (int i = 0; i < 4; i++) {
            sprintf(temp, "%d %c(%d) %d -> (%d) -> ", this->inWaitTime[this->route[i]], char('A' + this->route[i]),
                   this->playTime[this->route[i]], this->outWaitTime[this->route[i]], this->pathTime[this->route[i]]);
            ret += string(temp);
        }
        sprintf(temp, "Leave at %d, totally %d seconds. %d sites \n", this->leaveTime, this->leaveTime - this->arrivalTime,
        this->countSites);
        ret += string(temp);
        return ret;
    }
    // Define the comparation between two Tourist by their next event time value.
    // The one with smaller time will be arranged in the front of the Containers.
    bool operator < (const Tourist& b) const {
        return this->nextEventTime > b.nextEventTime;
    }
private:
    int id;

    // For Containers.
    int nextEventTime;

    // For Que.
    int enterQueTime;

    // For Containers.
    int enterConTime;

    // Records
    int countSites;
    int totalWaitTime;
    int arrivalTime, leaveTime;
    vector<int> inWaitTime, outWaitTime;
    vector<int> playTime, pathTime;
    vector<int> route;
};

// Defined for pointers of Tourist.
// This is also a workable code, replaced by the below template.
// When use this, change in Container: priority_queue<Tourist*, vector<Tourist*>, LessThan_Tourist<Tourist> > c;
//
//struct LessThan_Tourist
//{
//    bool operator()(const Tourist *a, const Tourist *b) const
//    {
//        return a->getNextEventTime() > b->getNextEventTime();
//    }
//};


// A template that define the two pointers comparison by mapping to the pointers object.
template<typename Type, typename Compare = std::less<Type> >
struct LessThan_Tourist : public std::binary_function<Type *, Type *, bool> {
    bool operator()(const Type *x, const Type *y) const
    { return Compare()(*x, *y); }
};

// Each object of this class is a queue of Tourists.
class Que {
public:
    // Enter the queue at the rear.
    void push(Tourist *a) { this->q.push(a); }
    // Return the first most element in the queue.
    Tourist* front() { return this->q.front(); }
    // Leave the queue at the head.
    void pop() { q.pop(); }
    // if the queue is empty;
    bool empty() { return q.empty(); }
    int getSize() { return q.size(); }
private:
    queue<Tourist*> q;
};

// Each object of this class is a container of Tourists.
// Container can output the minimium element in O(1) time.
class Container {
public:
    void addOne(Tourist *a) {
        this->c.push(a);
    }
    Tourist* top() { return this->c.top(); }
    void removeTop() { c.pop(); }
    bool empty() { return c.empty(); }
    int getSize() { return c.size(); }
private:
    // The smaller next event time toursits will have higher priority.
    priority_queue<Tourist*, vector<Tourist*>, LessThan_Tourist<Tourist> > c;
};


//TODO: Just for implementation test. Need more precise distribution.

class DataGenerate {
public:
    // All four sites use the same u and v for their distribution model.
    DataGenerate(string type, double u, double v) {
        if (type.compare("normal") != 0 && type.compare("exponential") != 0) {printf("Error initialize the DataGenerate\n"); exit(0);}
        this->type = type;
        init(u, v, u, v, u, v, u, v);
    }
    DataGenerate(string type, double u0, double v0, double u1, double v1, double u2, double v2, double u3, double v3) {
        if (type.compare("normal") != 0 && type.compare("exponential") != 0) {printf("Error initialize the DataGenerate\n"); exit(0);}
        this->type = type;
        init(u0, v0, u1, v1, u2, v2, u3, v3);
    }
    void init(double u0, double v0, double u1, double v1, double u2, double v2, double u3, double v3) {
        double u[] = {u0, u1, u2, u3};
        double v[] = {v0, v1, v2, v3};
        generator.resize(4);
        normalDistribution.resize(4);
        exponentialDistribution.resize(4);
        for (int i = 0; i < 4; i++) {
            this->generator[i] = new default_random_engine(rand());
            if (this->type == "normal") {
                this->normalDistribution[i] = new normal_distribution<double>(u[i], v[i]);
            } else if (this->type == "exponential") {
                this->exponentialDistribution[i] = new exponential_distribution<double>(u[i]);
            }
        }
    }
    double ret(int siteIndex) {
        if (this->type == "normal")
            return normalDistribution[siteIndex]->operator()(*generator[siteIndex]);
        else if (this->type == "exponential") {
            return exponentialDistribution[siteIndex]->operator()(*generator[siteIndex]);
        }
    }
private:
    string type;
    vector<default_random_engine*> generator;
    vector<normal_distribution<double>*> normalDistribution;
    vector<exponential_distribution<double>*> exponentialDistribution;
};

class DataReader {
public:
    DataReader(string dataName) {
        this->dataName = dataName;
        for (int i = 0; i < 4; i++) pos[i] = 0;
        for (int i = 0; i < 4; i++) {
            ifstream infile((string("data/") + dataName + string("/") + char('a' + i)).c_str());
            double t;
            while (infile >> t) {
                data[i].push_back(t);
            }
        }

    }
    double ret(int siteIndex) {
        if (this->pos[siteIndex] < this->data[siteIndex].size())
            return this->data[siteIndex][this->pos[siteIndex]++];
        else {
            cout << " Error in running: pointer exceeds the data's length." << endl;
            exit(0);
        }
    }
private:
    string dataName;
    int pos[4];
    vector<double> data[4];
};

int main() {
    srand(0);
    timeval startTime, endTime;
    gettimeofday(&startTime, 0);

    DataReader create("create");
    DataReader playTime("playtime");
    DataReader pathTime("pathTime");

    // Test distribution.
//    for (int i = 0; i < 100; i++) {
//        printf("%f ", create.ret(0));
//    }
//    printf("\n");
//    for (int i = 0; i < 100; i++) {
//        printf("%f ", playTime.ret(0));
//    }
//    printf("\n");
//    for (int i = 0; i < 100; i++) {
//        printf("%f ", pathTime.ret(0));
//    }
//    printf("\n");
//    return 0;

    vector<int> arriveTime[4];
    for (int i = 0; i < 4; i++) {
        for (int j = int(60 * create.ret(i)); j < MAX_SECONDS; j += int(60 * create.ret(i))) {
            if (j < MAX_SECONDS)
                arriveTime[i].push_back(j);
        }
    }
    for (int i = 0; i < 4; i++) {
        sort(arriveTime[i].begin(), arriveTime[i].end());
        for (int j = 0; j < arriveTime[i].size() && j < 100; j++) cout << arriveTime[i][j] << " ";
        cout << endl;
    }

    int tourist_id = 0;
    vector<Tourist*> allTourists;

    Que in_queue[4], out_queue[4];
    Container sites_container[4];
    Container paths_container[4];

    int docker_num[4], bicycle_num[4];

    for (int i = 0; i < 4; i++) {
        docker_num[i] = NUM_DOCKER;
        bicycle_num[i] = NUM_BICYCLE;
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

    int pos[4] = {0};
    // Run for MAX_SECONDS seconds.
    while (GLOBAL_TIME <= MAX_SECONDS) {
        //cout << GLOBAL_TIME << endl;

        // Update 4 sites situation at this second.
        for (int i = 0; i < 4; i++) {

            // The last site of site i.
            int last = (i - 1 >= 0) ? (i - 1) : 3;

            // Check if there are people arrived at site[i] from outside the theme park.
            while (pos[i] < arriveTime[i].size()) {
                int timestamp = arriveTime[i][pos[i]];
                if (timestamp != GLOBAL_TIME) {
                    break;
                }
                pos[i]++;
                Tourist *t = new Tourist(tourist_id++, GLOBAL_TIME + int(60 * playTime.ret(i)));
                // Plus 1 to the sites count of t.
                t->setCountSites(t->getCountSites() + 1);
                t->setArriveTime(GLOBAL_TIME);
                t->setEnterConTime(GLOBAL_TIME);
                t->appendRoute(i);
                allTourists.push_back(t);
                sites_container[i].addOne(t);
            }

            // Check if there are people arrived at site[i] from the last site.
            while (!paths_container[last].empty()) {
                // Tourist t is the one who will firstly finish paths[i] in the path container.
                Tourist *t = paths_container[last].top();
                if (t->getNextEventTime() != GLOBAL_TIME) break;
                paths_container[last].removeTop();
                // The tourist will go into the in queue,
                // so his leaving of queue depends on the num of bicycles at sites[i],
                // rather than a determined period of time. Here we set the next event time to -1.
                t->setNextEventTime(-1);
                // Calculate how long have been this container.
                t->setPathTime(last, GLOBAL_TIME - t->getEnterConTime());
                t->setEnterQueTime(GLOBAL_TIME);
                // Tourist t enters in the in queue of the next site.
                in_queue[i].push(t);
            }

            // Check if the in queue is emtpy and if there is empty docker.
            while (!in_queue[i].empty() && bicycle_num[i] < docker_num[i]) {
                // Tourist t is at the head of the in queue.
                Tourist *t = in_queue[i].front();
                in_queue[i].pop();
                // Tourist t return a bicycle.
                bicycle_num[i]++;
                // Calculate how long have the tourist been waiting in the queue.
                t->setInWaitTime(i, GLOBAL_TIME - t->getEnterQueTime());
                t->appendRoute(i);
                t->setTotalWaitTime(t->getTotalWaitTime() + (GLOBAL_TIME - t->getEnterQueTime()));
                // Set how long will the tourist play in sites[i].
                t->setNextEventTime(GLOBAL_TIME + int(60 * playTime.ret(i)));
                // Plus 1 to the sites count of t.
                t->setCountSites(t->getCountSites() + 1);
                // Set enter path time.
                t->setEnterConTime(GLOBAL_TIME);
                // Let Tourist t into sites[i].
                sites_container[i].addOne(t);
            }

            // Check if there are people want to out among the current tourists in sites[i].
            while (!sites_container[i].empty()) {
                // Tourist t is the one who will firstly leave sits[i] in the site container.
                Tourist *t = sites_container[i].top();
                if (t->getNextEventTime() != GLOBAL_TIME) break;
                sites_container[i].removeTop();
                // Calculate how long have been this container.
                t->setPlayTime(i, GLOBAL_TIME - t->getEnterConTime());
                // The tourist will go to a queue,
                // so his leaving of queue depends on the num of empty dockers,
                // rather than a determined period of time. Here we set the next event time to -1.
                t->setNextEventTime(-1);
                t->setEnterQueTime(GLOBAL_TIME);
                if (t->getCountSites() <= 3)
                    // Tourist t enters in the out queue.
                    out_queue[i].push(t);
                else
                    t->setLeaveTime(GLOBAL_TIME);
                // Else the tourist will leave this theme park.
            }

            // Check if the out queue is emtpy and if there is bicycle left.
            while (!out_queue[i].empty() && bicycle_num[i] > 0) {
                // Tourist t is at the head of the out queue.
                Tourist *t = out_queue[i].front();
                out_queue[i].pop();
                // Tourist t return a bicycle.
                bicycle_num[i]--;
                t->setOutWaitTime(i, GLOBAL_TIME - t->getEnterQueTime());
                // Calculate how long have the tourist been waiting in the queue.
                t->setTotalWaitTime(t->getTotalWaitTime() + (GLOBAL_TIME - t->getEnterQueTime()));
                // Set how long will the tourist spend on paths[i].
                t->setNextEventTime(GLOBAL_TIME + int(pathTime.ret(i) * 60));
                // Set enter path time.
                t->setEnterConTime(GLOBAL_TIME);
                // Tourist t on his way to the next site.
                paths_container[i].addOne(t);
            }
        }
        if (GLOBAL_TIME % 1000 == 0) {
            // Output the status of this second.
//            cout << "At second " << GLOBAL_TIME;
//            for (int i = 0; i < 4; i++) {
//                cout << "  " << char(i + 'A') << ": " << sites_container[i].getSize()
//                     << "  In " << char(i + 'A') << ": " << in_queue[i].getSize()
//                     << "  Out " << char(i + 'A') << ": " << out_queue[i].getSize();
//            }
//            cout << endl;
            printf("At second %d: ", GLOBAL_TIME);
            for (int i = 0; i < 4; i++) {
                printf("%d %c(%d) %d -> (%d) -> ", in_queue[i].getSize(), char('A' + i),
                       sites_container[i].getSize(), out_queue[i].getSize(), paths_container[i].getSize());
            }
            printf("A\n");
        }
        GLOBAL_TIME++;
    }

    gettimeofday(&endTime, 0);

    double total = 0;
    double maxWait = 0, maxWaitOneSite = 0;
    int countLessThen5Minutes = 0;
    int countFinish = 0;
    double aveInWaitTime[4] = {0.0}, aveOutWaitTime[4] = {0.0};
    for (auto each : allTourists) {
        if (each->getCountSites() != 4) continue;
        countFinish++;
        //cout << each->getTotalWaitTime() << " ";
        total += each->getAveWaitTime();
        maxWait = each->getAveWaitTime() > maxWait ? each->getAveWaitTime() : maxWait;
        if (each->getAveWaitTime() <= 5 * 60) {
            countLessThen5Minutes++;
        }
        for (int j = 0; j < 4; j++) {
            aveInWaitTime[j] += each->getInWaitTime(j);
            aveOutWaitTime[j] += each->getOutWaitTime(j);
            if (maxWaitOneSite < (each->getInWaitTime(j) + each->getOutWaitTime(j)))
                maxWaitOneSite = each->getInWaitTime(j) + each->getOutWaitTime(j);
        }
    }
    cout << "\n\n";
    cout << "* Generated " << allTourists.size() << " Tourists. " << countFinish << " finished their trip. \n";
    for (int i = 0; i < 4; i++) {
        printf("* At queue %c ave wait time: In %lf, Out %lf\n", char('A' + i),
               aveInWaitTime[i] / countFinish, aveOutWaitTime[i] / countFinish);
    }
    cout << "* Average waiting time each tourist per queue is " << total / countFinish << " seconds" << endl;
    cout << "* Max ave waiting time of a tourist is " << maxWait << " seconds." << endl;
    cout << "* Max waiting time at one site of a tourist is " << maxWaitOneSite << " seconds. " << endl;
    cout << "* " << (double(countLessThen5Minutes) / countFinish) * 100 << "% of toursits ave wait time less than 5 minutes.\n";


    long mtime, seconds, useconds;
    seconds  = endTime.tv_sec  - startTime.tv_sec;
    useconds = endTime.tv_usec - startTime.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    cout << "* Similated " << MAX_SECONDS / (24 * 3600) << " days.  "
         << "\n* Costed " << mtime << " ms to simulate.\n";

    ofstream outfile("tourist.txt");
    for (int i = 0; i < allTourists.size(); i++) {
        outfile << allTourists[i]->displayRoute();
    }
    cout << "Tourists route detail has been output to file tourist.txt." << endl;
    // Calculate the average waiting time.
    // ...
    return 0;
}
