#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>
#include <iostream>
#include <string>
#include "src/SimpleBuilder.h"

class ShurikenWindow {
private:
    Fl_Window *window;
    Fl_Input *sourceInput;
    Fl_Button *browseBtn;
    Fl_Button *buildBtn;
    Fl_Button *packageBtn;
    Fl_Text_Display *logDisplay;
    Fl_Text_Buffer *logBuffer;
    
    SimpleBuilder builder;
    std::string currentProject;

    static void browse_cb(Fl_Widget*, void* data) {
        ShurikenWindow* win = (ShurikenWindow*)data;
        win->browseSource();
    }
    
    static void build_cb(Fl_Widget*, void* data) {
        ShurikenWindow* win = (ShurikenWindow*)data;
        win->buildProject();
    }
    
    static void package_cb(Fl_Widget*, void* data) {
        ShurikenWindow* win = (ShurikenWindow*)data;
        win->packageProject();
    }

public:
    ShurikenWindow() : builder() {
        // Create main window
        window = new Fl_Window(800, 600, "Shuriken Lite - C++ IDE");
        window->begin();
        
        // Title
        Fl_Box *title = new Fl_Box(20, 20, 760, 40, "Shuriken Lite - Modern C++ Builder & IDE");
        title->labelfont(FL_BOLD);
        title->labelsize(18);
        title->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
        
        // Source input
        Fl_Box *sourceLabel = new Fl_Box(20, 80, 200, 25, "Project Source Folder:");
        sourceLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        
        sourceInput = new Fl_Input(20, 110, 600, 35);
        sourceInput->value("");
        
        browseBtn = new Fl_Button(640, 110, 140, 35, "Browse...");
        browseBtn->callback(browse_cb, this);
        
        // Build button
        buildBtn = new Fl_Button(20, 160, 200, 40, "Build Project");
        buildBtn->callback(build_cb, this);
        buildBtn->color(FL_BLUE);
        buildBtn->labelcolor(FL_WHITE);
        
        // Package button
        packageBtn = new Fl_Button(240, 160, 200, 40, "Create Installer");
        packageBtn->callback(package_cb, this);
        packageBtn->color(FL_DARK_GREEN);
        packageBtn->labelcolor(FL_WHITE);
        
        // Log display
        Fl_Box *logLabel = new Fl_Box(20, 220, 200, 25, "Build Output:");
        logLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        
        logBuffer = new Fl_Text_Buffer();
        logDisplay = new Fl_Text_Display(20, 250, 760, 330);
        logDisplay->buffer(logBuffer);
        logDisplay->wrap_mode(1, 0);
        
        window->end();
        window->resizable(logDisplay);
        
        // Initial log message
        logBuffer->append("Shuriken Lite IDE ready. Select a project folder and click Build.\n");
        logBuffer->append("Features: C++ compilation, installer creation, modern interface.\n\n");
    }
    
    ~ShurikenWindow() {
        delete logBuffer;
        delete window;
    }
    
    void show() {
        window->show();
    }
    
    void browseSource() {
        const char* dir = fl_dir_chooser("Select Project Source Folder", "");
        if (dir) {
            sourceInput->value(dir);
            currentProject = dir;
            
            logBuffer->append("Selected project: ");
            logBuffer->append(dir);
            logBuffer->append("\n");
        }
    }
    
    void buildProject() {
        std::string sourceDir = sourceInput->value();
        
        if (sourceDir.empty()) {
            fl_alert("Please select a source folder first!");
            return;
        }
        
        logBuffer->append("=== Building Project ===\n");
        logBuffer->append("Source: ");
        logBuffer->append(sourceDir.c_str());
        logBuffer->append("\n");
        
        // Build the project
        std::string buildLog;
        std::string outputExe = sourceDir + "/ShurikenBuild";
        
        bool success = builder.compileProject(sourceDir, outputExe, buildLog);
        
        logBuffer->append(buildLog.c_str());
        
        if (success) {
            logBuffer->append("\n✅ Build successful!\n");
            logBuffer->append("Executable: ");
            logBuffer->append(outputExe.c_str());
            logBuffer->append("\n\n");
            fl_message("Build completed successfully!");
        } else {
            logBuffer->append("\n❌ Build failed!\n\n");
            fl_alert("Build failed! Check the output for details.");
        }
        
        // Scroll to bottom
        logDisplay->scroll(logBuffer->length(), 0);
    }
    
    void packageProject() {
        if (currentProject.empty()) {
            fl_alert("Please build a project first!");
            return;
        }
        
        logBuffer->append("=== Creating Installer ===\n");
        logBuffer->append("This feature will create a distributable package.\n");
        logBuffer->append("Installer creation in development...\n\n");
        
        fl_message("Installer creation feature coming soon!\nYour executable is ready in the project folder.");
    }
    
    int run() {
        return Fl::run();
    }
};

int main(int argc, char **argv) {
    ShurikenWindow app;
    app.show();
    return app.run();
}