#include <iostream>
#include <string>
#include "hpdf.h"  
#include <iomanip> 

using namespace std;

class Student {
private:
    string name;
    string branch;
    string year;
    string course;
    string btID;
    string semester;
    int totalClasses;
    int attendedClasses;
    string dateOfIssue;  
    string gender;       

public:
    Student(string name, string branch, string year, string course, string btID, string semester, int totalClasses, int attendedClasses, string dateOfIssue, string gender) {
        this->name = name;
        this->branch = branch;
        this->year = year;
        this->course = course;
        this->btID = btID;
        this->semester = semester;
        this->totalClasses = totalClasses;
        this->attendedClasses = attendedClasses;
        this->dateOfIssue = dateOfIssue;
        this->gender = gender;  
    }

    float calculateAttendancePercentage() {
        if (totalClasses == 0) {
            return 0;
        }
        return (float(attendedClasses) / totalClasses) * 100;
    }

    void generatePDF() {
        HPDF_Doc pdf = HPDF_New(NULL, NULL);
        if (!pdf) {
            cout << "ERROR: Cannot create PDF object." << endl;
            return;
        }

        HPDF_Page page = HPDF_AddPage(pdf);
        HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

        // Header image
        HPDF_Image png = HPDF_LoadPngImageFromFile(pdf, "header.png");
        if (!png) {
            cout << "ERROR: Cannot load PNG image." << endl;
        } else {
            HPDF_Page_DrawImage(page, png, 50, 750, 500, 80);  
        }

        HPDF_Page_MoveTo(page, 50, 740);
        HPDF_Page_LineTo(page, 550,740);
        HPDF_Page_Stroke(page);

        HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
        HPDF_Page_SetFontAndSize(page, font, 14);

        //IIITN Attendance on left
        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, 50, 720, "IIITN Attendance/ 2024-25/");

        //Date on the right
        HPDF_Page_TextOut(page, 440, 720, ("Date: " + dateOfIssue).c_str());
        HPDF_Page_EndText(page);

        // Male ya Female ke accordingly MR ya MS ko "title" variable me store kar denge!
        string title;
        if (gender == "M" || gender == "m") {
            title = "Mr.";
        } 
        else {
            title = "Ms.";
        }

        HPDF_Page_BeginText(page);

        // paragraph 

        HPDF_Page_TextRect(page, 50, 670, 550, 570,
        ("This is to certify that (" + title + " " + name + ") (Enrollment Number " + btID + ") "
        "is a student of B.Tech, " + year + " year-A section (" + branch + " Department) at "
        "Indian Institute of Information Technology Nagpur. As per records, his overall attendance "
        "for the " + semester + " semester till date is as follows:").c_str(), HPDF_TALIGN_JUSTIFY, NULL);
        HPDF_Page_EndText(page);

        // Calculate attendance percentage and format it to two decimal places 
        float attendancePercentage = calculateAttendancePercentage();
        string attendanceStr = to_string(attendancePercentage).substr(0, 5) + "%";

        // Table below the paragraph
        HPDF_Page_SetLineWidth(page, 1.0);
        HPDF_Page_Rectangle(page, 50, 550, 500, 40);  // Table outline
        HPDF_Page_Stroke(page);

        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, 105, 575, "Semester Details");
        HPDF_Page_TextOut(page, 340, 575, "Attendance Percentage");
        HPDF_Page_EndText(page);

        HPDF_Page_MoveTo(page, 50, 570);
        HPDF_Page_LineTo(page, 550, 570);
        HPDF_Page_Stroke(page);

        HPDF_Page_MoveTo(page, 275, 590);
        HPDF_Page_LineTo(page, 275, 550);
        HPDF_Page_Stroke(page);

        // Table data
        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, 105, 555, (semester + " Semester").c_str());
        HPDF_Page_TextOut(page, 395, 555, attendanceStr.c_str());
        HPDF_Page_EndText(page);

        // Signature section
        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, 50, 500, "This certificate is issued on request for scholarship purpose.");
        HPDF_Page_TextOut(page, 50, 400, "Mrs. Puja V. Gudadhe");
        HPDF_Page_TextOut(page, 50, 385, "Faculty Advisor");
        HPDF_Page_TextOut(page, 50, 370, "3rd Year, CSE-A");

        HPDF_Page_TextOut(page, 50, 305, "Dr. Nishat Ansari");
        HPDF_Page_TextOut(page, 50, 290, "Head of the Department");
        HPDF_Page_TextOut(page, 50, 275, "Department of Computer Science and Engineering");
        HPDF_Page_TextOut(page, 50, 260, "IIIT Nagpur");
        HPDF_Page_EndText(page);

        string filename = btID + "_attendance_certificate.pdf";
        if (HPDF_SaveToFile(pdf, filename.c_str()) != HPDF_OK) {
            cout << "ERROR: Cannot save PDF file." << endl;
            HPDF_Free(pdf);
            return;
        }

        HPDF_Free(pdf);
        cout << "PDF file created successfully: " << filename << endl;
    }
};

Student getStudentData() {
    string name, branch, year, course, btID, semester, dateOfIssue, gender;
    int totalClasses, attendedClasses;

    cout << "Enter Name: ";
    getline(cin, name);

    cout << "Enter Branch: ";
    getline(cin, branch);

    cout << "Enter Year (First/Second/Third/Fourth): ";
    getline(cin, year);

    cout << "Enter Course: ";
    getline(cin, course);

    cout << "Enter BT ID: ";
    getline(cin, btID);

    cout << "Enter Current Semester: ";
    getline(cin, semester);

    cout << "Enter Total Classes: ";
    cin >> totalClasses;

    cout << "Enter Classes Attended: ";
    cin >> attendedClasses;

    cin.ignore();  // To ignore newline left in the input buffer

    cout << "Enter Date of Issue (dd/mm/yyyy): ";
    getline(cin, dateOfIssue);

    // Ask the user for gender (M/F)
    cout << "Enter Gender (M for Male, F for Female): ";
    getline(cin, gender);

    return Student(name, branch, year, course, btID, semester, totalClasses, attendedClasses, dateOfIssue, gender);
}

int main() {
    Student student = getStudentData();
    student.generatePDF();
    return 0;
}
