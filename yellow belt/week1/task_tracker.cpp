#include <iostream>
#include <map>
#include <vector>
using namespace std;

/*enum class TaskStatus {
    NEW,
    IN_PROGRESS,
    TESTING,
    DONE
};*/

TaskStatus Next(TaskStatus status) {
    return static_cast<TaskStatus>(static_cast<int>(status) + 1);
}

using TasksInfo = map<TaskStatus, int>;

void EraseZeros(TasksInfo& tasks) {
    vector <TaskStatus> statuses_to_delete;
    for (auto& [status, count] : tasks)
        if (count == 0)
            statuses_to_delete.push_back(status);

    for (auto status : statuses_to_delete)
        tasks.erase(status);
}

class TeamTasks {
private:
    map <string, TasksInfo> persons;
public:
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return persons.at(person);
    }

    void AddNewTask(const string& person) {
        persons[person][TaskStatus::NEW]++;
    }

    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
        TasksInfo updated_tasks, others_tasks;
        for (auto& [status, count] : persons[person]) {
            if (task_count == 0 || status == TaskStatus::DONE)
                break;

            int need_to_update = min(task_count, count);
            count -= need_to_update;
            task_count -= need_to_update;

            updated_tasks[Next(status)] = need_to_update;
        }
        EraseZeros(persons[person]);
        others_tasks = persons[person];

        persons[person].clear();
        for (auto& [status, count] : updated_tasks)
            persons[person][status] += count;
        for (auto& [status, count] : others_tasks)
            persons[person][status] += count;

        others_tasks.erase(TaskStatus::DONE);

        return make_tuple(updated_tasks, others_tasks);
    }
};


void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
         ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
         ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
         ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    return 0;
}

