#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

// 课程信息结构体
struct Course {
	string name;     // 课程名
	double credit;   // 学分（支持两位小数）
	int score;       // 百分制分数
	double gpa;      // 绩点
};

// 根据百分制分数返回绩点的函数
double scoreToGPA(int score) {
	if (score >= 96 && score <= 100) return 4.8;
	else if (score >= 93 && score <= 95) return 4.5;
	else if (score >= 90 && score <= 92) return 4.0;
	else if (score >= 86 && score <= 89) return 3.8;
	else if (score >= 83 && score <= 85) return 3.5;
	else if (score >= 80 && score <= 82) return 3.0;
	else if (score >= 76 && score <= 79) return 2.8;
	else if (score >= 73 && score <= 75) return 2.5;
	else if (score >= 70 && score <= 72) return 2.0;
	else if (score >= 66 && score <= 69) return 1.8;
	else if (score >= 63 && score <= 65) return 1.5;
	else if (score >= 60 && score <= 62) return 1.0;
	else return 0.0;
}

// 检查输入是否为结束命令
bool isEndCommand(const string& input) {
	string lowerInput = input;
	transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);
	return (lowerInput == "计算" || lowerInput == "calculate" || lowerInput == "end" || lowerInput == "exit");
}

// 验证学分是否合法
bool isValidCredit(double credit) {
	return credit > 0 && credit <= 20;  // 假设学分上限为20
}

// 验证分数是否合法
bool isValidScore(int score) {
	return score >= 0 && score <= 100;
}

// 输入课程信息
vector<Course> inputCourses() {
	vector<Course> courses;
	string inputLine;
	int lineCount = 0;

	cout << "========================================" << endl;
	cout << "    GPA计算程序 - by东南大学吴健雄学院夏滔" << endl;
	cout << "========================================" << endl;
	cout << "\n使用说明：" << endl;
	cout << "1. 请输入课程信息，格式为：课程名 学分 百分制分数" << endl;
	cout << "2. 学分支持两位小数（如：0.25, 1.5, 3.0, 4.5等）" << endl;
	cout << "3. 每行输入一门课程信息" << endl;
	cout << "4. 输入完成后，在新的一行输入 \"计算\" 或 \"calculate\" 开始计算" << endl;
	cout << "5. 您也可以输入 \"end\" 或 \"exit\" 结束输入" << endl;
	cout << "\n示例：" << endl;
	cout << "高等数学 4.0 85" << endl;
	cout << "实验课 0.25 90" << endl;
	cout << "C程序设计 3.0 92" << endl;
	cout << "大学英语 2.0 78" << endl;
	cout << "计算" << endl;
	cout << "========================================" << endl;
	cout << "\n请开始输入课程信息：" << endl;

	while (true) {
		lineCount++;
		cout << "行" << lineCount << "> ";

		// 读取整行输入
		getline(cin, inputLine);

		// 跳过空行
		if (inputLine.empty()) {
			lineCount--;
			continue;
		}

		// 检查是否为结束命令
		if (isEndCommand(inputLine)) {
			if (courses.empty()) {
				cout << "未输入任何课程信息，请至少输入一门课程！" << endl;
				continue;
			}
			cout << "输入结束，开始计算..." << endl;
			break;
		}

		// 使用字符串流解析输入
		stringstream ss(inputLine);
		string name;
		double credit;
		int score;

		// 尝试解析输入
		if (ss >> name >> credit >> score) {
			// 验证学分和分数的有效性
			if (!isValidCredit(credit)) {
				cout << "错误：学分必须在0-20之间，请重新输入此行" << endl;
				lineCount--;
				continue;
			}

			if (!isValidScore(score)) {
				cout << "错误：分数必须在0-100之间，请重新输入此行" << endl;
				lineCount--;
				continue;
			}

			// 创建课程对象
			Course course;
			course.name = name;
			course.credit = credit;
			course.score = score;

			// 添加到课程列表
			courses.push_back(course);
			cout << "  已添加: " << name << " (" << fixed << setprecision(2)
				<< credit << "学分, " << score << "分)" << endl;
		} else {
			cout << "格式错误！请使用格式：课程名 学分 分数" << endl;
			cout << "示例: 高等数学 4.0 85" << endl;
			cout << "或: 实验课 0.25 90" << endl;
			lineCount--;
		}
	}

	return courses;
}

// 计算每门课的绩点
void calculateGPAs(vector<Course>& courses) {
	for (auto& course : courses) {
		course.gpa = scoreToGPA(course.score);
	}
}

// 计算总GPA
double calculateOverallGPA(const vector<Course>& courses) {
	double totalWeighted = 0.0;  // 加权绩点总和
	double totalCredits = 0.0;   // 总学分

	for (const auto& course : courses) {
		totalWeighted += course.gpa * course.credit;
		totalCredits += course.credit;
	}

	return (totalCredits > 0) ? totalWeighted / totalCredits : 0.0;
}

// 显示每门课的绩点
void displayCourseGPAs(const vector<Course>& courses) {
	cout << "\n========== 各门课绩点 ==========" << endl;
	cout << left << setw(4) << "序号"
		<< setw(20) << "课程名"
		<< setw(10) << "学分"
		<< setw(10) << "分数"
		<< "绩点" << endl;
	cout << "--------------------------------" << endl;

	for (size_t i = 0; i < courses.size(); i++) {
		const auto& course = courses[i];
		cout << left << setw(4) << i + 1
			<< setw(20) << course.name
			<< setw(10) << fixed << setprecision(2) << course.credit
			<< setw(10) << course.score
			<< fixed << setprecision(2) << course.gpa << endl;
	}
}

// 显示总GPA计算结果
void displayOverallGPA(const vector<Course>& courses, double overallGPA) {
	double totalWeighted = 0.0;
	double totalCredits = 0.0;

	for (const auto& course : courses) {
		totalWeighted += course.gpa * course.credit;
		totalCredits += course.credit;
	}

	cout << "\n========== 总GPA计算 ==========" << endl;
	cout << "已计算课程数: " << courses.size() << endl;
	cout << "加权绩点总和: " << fixed << setprecision(3) << totalWeighted << endl;
	cout << "总学分: " << fixed << setprecision(2) << totalCredits << endl;
	cout << "------------------------------" << endl;
	cout << "您的总GPA为: " << fixed << setprecision(4) << overallGPA << endl;

	// 提供GPA等级参考
	cout << "\n========== GPA等级参考 ==========" << endl;
	if (overallGPA >= 4.5) {
		cout << "您的GPA非常优秀！" << endl;
	} else if (overallGPA >= 3.5) {
		cout << "您的GPA良好，继续努力！" << endl;
	} else if (overallGPA >= 2.5) {
		cout << "您的GPA中等，有提升空间！" << endl;
	} else if (overallGPA >= 1.5) {
		cout << "您的GPA及格，需要更加努力！" << endl;
	} else {
		cout << "您的GPA有待提高，请继续努力！" << endl;
	}
}

// 保存结果到文件
void saveResultToFile(const vector<Course>& courses, double overallGPA) {
	char choice;
	cout << "\n是否将结果保存到文件？(y/n): ";
	cin >> choice;
	cin.ignore();  // 清除缓冲区中的换行符

	if (choice == 'y' || choice == 'Y') {
		string filename;
		cout << "请输入文件名（如: gpa_result.txt）: ";
		getline(cin, filename);

		ofstream outFile(filename);
		if (!outFile) {
			cout << "无法创建文件！" << endl;
			return;
		}

		outFile << "========== GPA计算结果 ==========" << endl;
		outFile << "生成时间: " << __DATE__ << " " << __TIME__ << endl;
		outFile << "课程数: " << courses.size() << endl;
		outFile << "\n--------------------------------" << endl;
		outFile << left << setw(4) << "序号"
			<< setw(20) << "课程名"
			<< setw(10) << "学分"
			<< setw(10) << "分数"
			<< "绩点" << endl;
		outFile << "--------------------------------" << endl;

		for (size_t i = 0; i < courses.size(); i++) {
			const auto& course = courses[i];
			outFile << left << setw(4) << i + 1
				<< setw(20) << course.name
				<< setw(10) << fixed << setprecision(2) << course.credit
				<< setw(10) << course.score
				<< fixed << setprecision(2) << course.gpa << endl;
		}

		outFile << "--------------------------------" << endl;
		outFile << "加权绩点总和: " << fixed << setprecision(3)
			<< calculateOverallGPA(courses) * (courses.empty() ? 0 :
				[&]() { double sum = 0; for (const auto& c : courses) sum += c.credit; return sum; }()) << endl;

		double totalCredits = 0;
		for (const auto& course : courses) {
			totalCredits += course.credit;
		}
		outFile << "总学分: " << fixed << setprecision(2) << totalCredits << endl;
		outFile << "总GPA: " << fixed << setprecision(4) << overallGPA << endl;

		outFile.close();
		cout << "结果已保存到 " << filename << endl;
	}
}

// 显示绩点对照表
void displayGPATable() {
	char choice;
	cout << "\n是否查看绩点对照表？(y/n): ";
	cin >> choice;
	cin.ignore();  // 清除缓冲区中的换行符

	if (choice == 'y' || choice == 'Y') {
		cout << "\n========== 东南大学绩点对照表 ==========" << endl;
		cout << "分数段      绩点" << endl;
		cout << "100-96      4.8" << endl;
		cout << "95-93       4.5" << endl;
		cout << "92-90       4.0" << endl;
		cout << "89-86       3.8" << endl;
		cout << "85-83       3.5" << endl;
		cout << "82-80       3.0" << endl;
		cout << "79-76       2.8" << endl;
		cout << "75-73       2.5" << endl;
		cout << "72-70       2.0" << endl;
		cout << "69-66       1.8" << endl;
		cout << "65-63       1.5" << endl;
		cout << "62-60       1.0" << endl;
		cout << "<60         0.0" << endl;
		cout << "======================================" << endl;
	}
}

// 主函数
int main() {
	// 输入课程信息
	vector<Course> courses = inputCourses();

	// 计算每门课的绩点
	calculateGPAs(courses);

	// 显示每门课的绩点
	displayCourseGPAs(courses);

	// 计算总GPA
	double overallGPA = calculateOverallGPA(courses);

	// 显示总GPA计算结果
	displayOverallGPA(courses, overallGPA);

	// 显示绩点对照表
	displayGPATable();

	// 保存结果到文件
	saveResultToFile(courses, overallGPA);

	cout << "\n========================================" << endl;
	cout << "       感谢使用GPA计算程序！" << endl;
	cout << "       祝您学业进步，前程似锦！" << endl;
	cout << "========================================" << endl;

	// 等待用户按回车键退出
	cout << "\n按回车键退出程序...";
	cin.get();

	return 0;

}
