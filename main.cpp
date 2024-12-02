#include "Base.h"

class Task {
public:
	Task(const std::string& name = "New Task", const std::string& desc = "", int priority = 0) : m_name(name), m_description(desc), m_priority(priority) {}
	~Task(){}

	inline std::string getName() const { return m_name; }
	inline std::string getDescription() const { return m_description; }
	inline int getPriority() const { return m_priority; }

	bool operator==(const Task& other) const {
		return m_name == other.getName() && m_description == other.getDescription() && m_priority == other.getPriority();
	}

private:
	std::string m_name;
	std::string m_description;
	int m_priority;

	// TODO: end time
};

class TaskManager {
public:
	TaskManager(){
		loadTasks();
		sort();
	}
	~TaskManager(){}

	void loadTasks() {
		std::ifstream file(ALL_TASKS_FILE_NAME);
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				std::istringstream iss(line);
				std::string n, d, pr;

				if (std::getline(iss, n, '~') && std::getline(iss, d, '~') && std::getline(iss, pr)) {
					n = n.substr(1, n.size() - 2);
					d = d.substr(1, d.size() - 2);
					int p = std::stoi(pr);

					m_allTasks.push_back(new Task(n, d, p));
				}
			}
			file.close();
		}
		else
			std::cout << "Error: couldn't to open file with all tasks!";
	}

	void addTask(Task* task) {
		for (const auto& t : m_allTasks) {
			if (t == task) {
				std::cout << "\n\nTask already exist in the file.\n\n";
				return;
			}
		}

		m_allTasks.push_back(task);
		sort();

		std::ofstream fOut(ALL_TASKS_FILE_NAME, std::ios::app);
		if (fOut.is_open()) {
			fOut << "\"" << (task->getName()) << "\"~\"" << (task->getDescription()) << "\"~" << task->getPriority() << "\n";
			fOut.close();
		}
		else {
			std::cout << "\nError: couldn't to open file with all tasks!\n";
		}
	}

	void removeTask(int index) {
		if (index >= m_allTasks.size() || index < 0) {
			std::cout << "\nError: this index not in array!\n";
			return;
		}
		Task* task = m_allTasks[index];

		std::ofstream tmpO("tmp.txt");
		std::ifstream fileI(ALL_TASKS_FILE_NAME);

		std::string tmpLine;

		if (tmpO.is_open()) {
			while (std::getline(fileI, tmpLine)) tmpO << tmpLine << '\n';
			tmpO.close();
		}
		fileI.clear();
		fileI.close();
		remove(ALL_TASKS_FILE_NAME.c_str());
		
		std::vector<Task*> tmpTasks;
		std::string deleteString = "\"" + (task->getName()) + "\"~\"" + task->getDescription() + "\"~" + (std::to_string(task->getPriority())) + "\n";

		std::ifstream tmpI("tmp.txt");
		std::ofstream fileO(ALL_TASKS_FILE_NAME);

		tmpTasks = m_allTasks;
		m_allTasks.clear();

		if (tmpI.is_open()) {
			std::string line; int i = 0;
			while (std::getline(tmpI, line)) {
				if (line+'\n' != deleteString) {
					fileO << line << "\n";
					m_allTasks.push_back(tmpTasks[i]);
				}
				++i;
			}
			fileO.close();
			tmpI.close();
			remove("tmp.txt");
		}

		sort();
	}

	void sort() {
		for (int i = 0; i < m_allTasks.size(); i++) {
			for (int j = 1; j < m_allTasks.size() - i; j++) {
				Task* tmp;
				if (m_allTasks[j - 1]->getPriority() < m_allTasks[j]->getPriority()) {
					tmp = m_allTasks[j - 1];
					m_allTasks[j - 1] = m_allTasks[j];
					m_allTasks[j] = tmp;
				}
			}
		}
	}

	int getTasksCount() { return m_allTasks.size(); }

	void displayTasks() {
		system("cls");
		std::cout << "Your current tasks:\n";
		std::cout << "-----------------------------------------------\n\n";
		for (int i = 1; i <= m_allTasks.size(); i++) {
			std::cout << "\t" << i << ") " << m_allTasks[i - 1]->getName() << '\n';
			std::cout << "\t   " << m_allTasks[i - 1]->getDescription() << "\n\n";
		}
		std::cout << "-----------------------------------------------\n\n";
		std::cout << "That's all for now!\n";
	}

private:
	std::vector<Task*> m_allTasks;
};

int main() {
#ifdef PLATFORM_WINDOWS
	CoInitialize(nullptr);
	showNotification(L"Task Name", L"description");
#endif

	TaskManager manager;

	Task* newTask;
	while (true) {
		manager.displayTasks();
		std::cout << "\n\nDo you want to add/remove a new task? Press +/-\n";

		char input = getchar();

		if (input == '+') {
			std::string n, d;
			int p;

			std::cout << "\tTask Name: "; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::getline(std::cin, n);
			std::cout << "\tTask Description: "; std::getline(std::cin, d);
			std::cout << "\tTask Priority: "; std::cin >> p;

			newTask = new Task(n, d, p);
			manager.addTask(newTask);
		}
		else if (input == '-') {
			int i;
			std::cout << "\tTask index (from 1 to " << manager.getTasksCount() << "): "; std::cin >> i;
			manager.removeTask(i-1);
		}
	}

	delete newTask;

#ifdef PLATFORM_WINDOWS
	CoUninitialize();
#endif
	return 0;
}