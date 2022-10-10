#include<bits/stdc++.h>
#include<windows.h>
#include<unistd.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct Question{
    string question;
    vector<string> options;
    int rightOption;
    Question(string _question, vector<string> _options, int _rightOption): question(_question), options(_options), rightOption(_rightOption){}
};

class ProgressBar {
public:
    void update(double newProgress) {
        currentProgress += newProgress;
        amountOfFiller = (int)((currentProgress / neededProgress)*(double)pBarLength);
    }
    void print() {
        currUpdateVal %= pBarUpdater.length();
        cout << "\r"
            << firstPartOfpBar;
        for (int a = 0; a < amountOfFiller; a++) {
            cout << pBarFiller;
        }
        cout << pBarUpdater[currUpdateVal];
        for (int b = 0; b < pBarLength - amountOfFiller; b++) {
            cout << " ";
        }
        string status = (int)(100*(currentProgress/neededProgress)) == 100 ? " Completed" : " File loading";
        cout << lastPartOfpBar
            << status << "(" << (int)(100*(currentProgress/neededProgress)) << "%)"
            << flush;
        currUpdateVal += 1;
    }
    std::string firstPartOfpBar = "[",
        lastPartOfpBar = "]",
        pBarFiller = "=",
        pBarUpdater = ">";
    private:
        int amountOfFiller,
            pBarLength = 50,
            currUpdateVal = 0;
        double currentProgress = 0,
            neededProgress = 100;
};

std::string space_to_underscore(std::string text) {
    std::replace(text.begin(), text.end(), ' ', '_');
    return text;
}

void show_question_instructions() {
    cout<<"\n\n\nInstruction: You have to provide few things to set a quiz. They are: \n";
    cout<<"1. Questions\n";
    cout<<"2. 4 options\n";
    cout<<"3. Right answer among the options(1/2/3/4)\n";
    cout<<"\nAre you interested to continue?[y/n]:";
}

void show_test_instructions() {
    cout<<"\n\n\nInstruction: You have to follow few things to participate a quiz. They are: \n";
    cout<<"1. Choose question set\n";
    cout<<"2. Read the question and options\n";
    cout<<"3. choose an option in between 1 to 4. Disqualified otherwise\n";
    cout<<"\nAre you interested to continue?[y/n]:";
}

vector<Question*> get_quiz_questions_from_setter(int numOfQuestions) {
    vector<Question*> allQuestions;
    for(int j = 1; j <= numOfQuestions; j++) {
        cout<<"Question-"<<j<<": ";
        string question;
        vector<string> options(4);
        int rightOption;
        getline(cin, question);
        for(int i = 0; i < 4; i++) {
            cout << "Option-" << i+1 << ": ";
            getline(cin, options[i]);
        }
        while(1) {
            cout<<"Right option[1/2/3/4]: ";
            cin >> rightOption;
            if(rightOption >0 && rightOption <5) break;
        }
        Question* q = new Question(question, options, rightOption);
        allQuestions.push_back(q);
        if(j < numOfQuestions) cin.ignore();
    }
    return allQuestions;
}

void save_quiz_in_a_file(vector<Question*> &allQuestions) {
    string quizName;
    cout << "\nName of your quiz: ";
    cin.ignore();
    getline(cin, quizName);
    quizName = space_to_underscore(quizName);
    string fileName = quizName+".txt";
    ofstream file_;
    file_.open(fileName);
    for(int i = 0; i < allQuestions.size(); i++) {
        Question* q = allQuestions[i];
        vector<string> options = q->options;
        file_ << q->question << "\n";
        for(int _i = 0; _i < options.size(); _i++) {
            file_ << options[_i] << "\n";
        }
        file_ << q->rightOption << "\n";
    }
    file_.close();
}

void set_question() {
    char isInterested;
    int numOfQuestions;
    show_question_instructions();
    cin>>isInterested;
    if(isInterested == 'n' || isInterested == 'N') {
        cout<<"\n\n\n";
        return;
    }
    cout<<"\nNumber of questions in the quiz: ";
    cin>>numOfQuestions;
    cin.ignore();
    vector<Question*> allQuestions = get_quiz_questions_from_setter(numOfQuestions);
    save_quiz_in_a_file(allQuestions);
}

vector<string> get_all_files_names_within_folder() {
    char cwd[256];
    getcwd(cwd, 256);
    string cwd_str = string(cwd);
    for(int i = 0; cwd_str[i]; i++) if(cwd_str[i] == '\\') cwd_str[i] = '/';
    string folder = cwd_str;
    vector<string> names;
    string search_path = folder + "/*.txt";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                string name = fd.cFileName;
                name = name.substr(0, name.size() - 4);
                names.push_back(name);
            }
        }while(::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return names;
}

string show_quiz_sets_and_get_selected_quiz() {
    vector<string> files = get_all_files_names_within_folder();
    cout<<"Available quizes: \n";
    for(int i = 0; i < files.size(); i++) {
        cout<<"["<<i+1<<"] "<<files[i]<<endl;
    }
    int selcetedNum = 0;
    if(files.size() > 0) {
        while(1) {
            cout<<"Choose your quiz set[1, "<<files.size()<<"]: ";
            cin>>selcetedNum;
            if(selcetedNum > 0 && selcetedNum <= files.size()) break;
        }
        return files[selcetedNum-1];
    }
    return "";
}

vector<Question*> get_quiz_questions_from_file(string file) {
    int rightOption;
    string tp, question;
    vector<string> options;
    vector<Question*> res;
    fstream file_;
    file_.open(file, ios::in);
    int lineCount = 1;
    while(getline(file_, tp)) {
        if(lineCount%6 == 1) question = tp;
        else if(lineCount%6 >= 2 && lineCount%6 <= 5) options.push_back(tp);
        else {
            if(tp.compare("1") == 0) rightOption = 1;
            if(tp.compare("2") == 0) rightOption = 2;
            if(tp.compare("3") == 0) rightOption = 3;
            if(tp.compare("4") == 0) rightOption = 4;
            Question* q = new Question(question, options, rightOption);
            res.push_back(q);
            options.clear();
        }
        ++lineCount;
    }
    file_.close();
    return res;
}

float start_exam(vector<Question*> &questions) {
    int score = 0;
    for(int i = 0; i < questions.size(); i++) {
        cout << "Question: " << questions[i]->question << endl;
        string ans = questions[i]->options[questions[i]->rightOption-1];

        srand(time(NULL));
        int randomVal = rand() % 10;
        for(int a = 0; a < randomVal; a++) random_shuffle(questions[i]->options.begin(), questions[i]->options.end());
        int selecedAns;
        for(int j = 0; j < questions[i]->options.size(); j++) {
            cout << "["<<j+1<<"]: " << questions[i]->options[j] << endl;
        }
        cout << "Your choice[1/2/3/4]: ";
        cin >> selecedAns;
        if(selecedAns > 0 && selecedAns < 5) {
            if(ans.compare(questions[i]->options[selecedAns-1]) == 0) {
                score++;
                cout << "[ Correct answer ]" << endl;
            } else {
                cout << "[ Wrong answer ]" << endl;
                cout << "[ Correct answer ]: " << ans << endl;
            }
        } else {
            cout << "You are disqualified\n" << endl;
            score = 0;
            break;
        }
    }
    return (score * 100) / questions.size();
}

void take_exam() {
    char isInterested;
    show_test_instructions();
    cin>>isInterested;
    if(isInterested == 'n' || isInterested == 'N') {
        cout<<"\n\n\n";
        return;
    }
    string selectedQuizFileName = show_quiz_sets_and_get_selected_quiz()+".txt";
    if(selectedQuizFileName.compare(".txt") == 0) {
        cout << "No quiz has been set yet" << endl;
        return;
    }
    vector<Question*> allQuestions = get_quiz_questions_from_file(selectedQuizFileName);

    ProgressBar bar;
    for (int i = 0; i < 4; i++) {
        bar.update(25);
        bar.print();
        sleep(1);
    }
    cout << endl;

    float score = start_exam(allQuestions);
    cout << "\n\n\n\nScore: " << score << "%\n";
    if(score >= 80) cout << "Good Job! Keep it up!" << endl;
    else if(score >= 50) cout << "Above average. Try hard." << endl;
    else cout << "Better luck next time." << endl;
}

int main() {
    cout << "1. Set a quiz\n";
    cout << "2. Participate a quiz\n";
    cout << "3. Exit\n";
    cout << "Choose an option(1/2/3):";
    int option;
    cin >> option;
    if(option == 1) set_question();
    else if(option == 2) take_exam();
    else if (option == 3) return 0;
    else cout << "Wrong choice\n";
    main();
    return 0;
}
