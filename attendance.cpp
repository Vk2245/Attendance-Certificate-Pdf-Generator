#include <iostream>
#include <string>
#include "hpdf.h"  // Make sure to link the Haru PDF library
#include <iomanip> // For rounding percentage

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

public:
    // Constructor to initialize the Student object
    Student(string name, string branch, string year, string course, string btID, string semester, int totalClasses, int attendedClasses) {
        this->name = name;
        this->branch = branch;
        this->year = year;
        this->course = course;
        this->btID = btID;
        this->semester = semester;
        this->totalClasses = totalClasses;
        this->attendedClasses = attendedClasses;
    }

    // Member function to calculate attendance percentage
    float calculateAttendancePercentage() {
        if (totalClasses == 0) {
            return 0;
        }
        return (float(attendedClasses) / totalClasses) * 100;
    }

    // Function to generate the PDF certificate
    void generatePDF() {
        // Create a new PDF document
        HPDF_Doc pdf = HPDF_New(NULL, NULL);
        if (!pdf) {
            cout << "ERROR: Cannot create PDF object." << endl;
            return;
        }

        // Add a new page to the PDF
        HPDF_Page page = HPDF_AddPage(pdf);

        // Set page to portrait orientation
        HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

        // Load the header image (college banner)
        HPDF_Image png = HPDF_LoadPngImageFromFile(pdf, "header.png");
        if (!png) {
            cout << "ERROR: Cannot load PNG image." << endl;
        } else {
            // Draw the image at the top with margins
            HPDF_Page_DrawImage(page, png, 50, 750, 500, 80);
        }

        // Set font and size
        HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
        HPDF_Page_SetFontAndSize(page, font, 14);

        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, 50, 730, "IIITN Attendance 2024-25");
        HPDF_Page_EndText(page);

        // Start writing the paragraph with proper margins
        HPDF_Page_BeginText(page);
        HPDF_Page_TextRect(page, 50, 700, 550, 600,
            ("This is to certify that (Mr./Ms. " + name + ") (Enrollment Number " + btID + ") "
            "is a student of B.Tech, " + year + " year-A section (" + branch + " Department) at "
            "Indian Institute of Information Technology Nagpur. As per records, his overall attendance "
            "for the " + semester + " semester till date is as follows:").c_str(), HPDF_TALIGN_LEFT, NULL);

        HPDF_Page_EndText(page);

        // Calculate attendance percentage and round to 2 decimal places
        float attendancePercentage = calculateAttendancePercentage();
        string attendanceStr = to_string(attendancePercentage).substr(0, 5) + "%"; // Show 2 digits after decimal

        // Draw the table below the paragraph
        HPDF_Page_SetLineWidth(page, 1.0);
        HPDF_Page_Rectangle(page, 50, 550, 500, 40);  // Outer table rectangle
        HPDF_Page_Stroke(page);

        // Table headers
        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, 105, 575, "Semester Details");
        HPDF_Page_TextOut(page, 340, 575, "Attendance Percentage");
        HPDF_Page_EndText(page);

        HPDF_Page_MoveTo(page, 50, 570);
        HPDF_Page_LineTo(page, 550, 570);
        HPDF_Page_Stroke(page);

        HPDF_Page_MoveTo(page, 275,590 );
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

        HPDF_Page_TextOut(page, 50, 320, "Dr. Nishat Ansari");
        HPDF_Page_TextOut(page, 50, 305, "Head of the Department");
        HPDF_Page_TextOut(page, 50, 290, "Department of Computer Science and Engineering");
        HPDF_Page_TextOut(page, 50, 275, "IIIT Nagpur");
        HPDF_Page_EndText(page);

        // Save the PDF to a file
        string filename = btID + "_attendance_certificate.pdf";
        if (HPDF_SaveToFile(pdf, filename.c_str()) != HPDF_OK) {
            cout << "ERROR: Cannot save PDF file." << endl;
            HPDF_Free(pdf);
            return;
        }

        // Free the PDF document
        HPDF_Free(pdf);
        cout << "PDF file created successfully: " << filename << endl;
    }
};

// Function to collect user input and return a Student object
Student getStudentData() {
    string name, branch, year, course, btID, semester;
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

    return Student(name, branch, year, course, btID, semester, totalClasses, attendedClasses);
}

int main() {
    // Create a Student object using the getStudentData function
    Student student = getStudentData();

    // Generate a PDF certificate
    student.generatePDF();

    return 0;
}
