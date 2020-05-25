#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Request {
    Request(int arrival_time, int process_time):
        arrival_time(arrival_time),
        process_time(process_time)
    {}

    int arrival_time;
    int process_time;
};

struct Response {
    Response(bool dropped, int start_time):
        dropped(dropped),
        start_time(start_time)
    {}

    bool dropped;
    int start_time;
};

class Buffer {
public:
    Buffer(int size):
        size_(size),
        finish_time_()
    {}

    Response Process(const Request &request) {
        // write your code here
        bool dropped = false;
        int start_time;

        while(!this->finish_time_.empty() && this->finish_time_.front() <= request.arrival_time)
        {
          this->finish_time_.pop();
        }

        if(!this->finish_time_.empty())
        {
          int last_finish_time = this->finish_time_.back();
          if(last_finish_time <= request.arrival_time)
          {
            start_time = request.arrival_time;
          }
          else
          {
            if(this->finish_time_.size() >= this->size_)
            {
              dropped = true;
            }
            else
            {
              start_time = last_finish_time;
            }
          }
        }
        else
        {
          start_time = request.arrival_time;
        }

        if(!dropped)
        {
          this->finish_time_.push(start_time + request.process_time);
        }

        return Response(dropped, start_time);
    }
private:
    int size_;
    std::queue <int> finish_time_;
};

std::vector <Request> ReadRequests() {
    std::vector <Request> requests;
    int count;
    std::cin >> count;
    for (int i = 0; i < count; ++i) {
        int arrival_time, process_time;
        std::cin >> arrival_time >> process_time;
        requests.push_back(Request(arrival_time, process_time));
    }
    return requests;
}

std::vector <Response> ProcessRequests(const std::vector <Request> &requests, Buffer *buffer) {
    std::vector <Response> responses;
    for (int i = 0; i < requests.size(); ++i)
        responses.push_back(buffer->Process(requests[i]));
    return responses;
}

void PrintResponses(const std::vector <Response> &responses) {
    for (int i = 0; i < responses.size(); ++i)
        std::cout << (responses[i].dropped ? -1 : responses[i].start_time) << std::endl;
}

int main() {
    int size;
    std::cin >> size;
    std::vector <Request> requests = ReadRequests();

    Buffer buffer(size);
    std::vector <Response> responses = ProcessRequests(requests, &buffer);

    PrintResponses(responses);
    return 0;
}
