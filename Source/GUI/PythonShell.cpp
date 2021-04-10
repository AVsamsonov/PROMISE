//
// Created by Alex Samsonov on 5/04/21.
//

#include "PythonShell.h"
#include <pybind11/embed.h>
namespace py = pybind11;

//==============================================================================
struct StdoutCatcher {
    juce::TextEditor *outpudDisplay;
    explicit StdoutCatcher(juce::TextEditor *outpudDisplayPtr) {outpudDisplay=outpudDisplayPtr;}
    void write(const std::string &str) {
        outpudDisplay->moveCaretToEnd();
        outpudDisplay->insertTextAtCaret(str);
    }
};

PYBIND11_EMBEDDED_MODULE(stdoutredirect, m) {
    py::class_<StdoutCatcher>(m, "StdoutCatcher")
            .def(py::init<juce::TextEditor *>())
            .def("write", &StdoutCatcher::write);
}

//==============================================================================
PythonShell::PythonShell()
{
    outputDisplay.setMultiLine(true);
    outputDisplay.setReadOnly(true);
    outputDisplay.setCaretVisible (false);
    outputDisplay.setFont({ Font::getDefaultMonospacedFontName(), 14.0f, Font::plain });
    addAndMakeVisible(outputDisplay);

    addAndMakeVisible(resizer);

    editor.setFont({ juce::Font::getDefaultMonospacedFontName(), 14.0f, juce::Font::plain });
    editor.setTabSize(4, true);
    editor.addKeyListener(this);
    addAndMakeVisible(editor);

    layout.setItemLayout(0, -0.2, -1, -1);
    layout.setItemLayout(1, 4, 4, 4);
    layout.setItemLayout(2, -0.2, -1, -1);

    // Redirect stdout from python to the outputDisplay
    auto stdoutredirect = py::module_::import("stdoutredirect");
    auto sys = py::module_::import("sys");
    sys.attr("stdout") = StdoutCatcher(std::addressof(outputDisplay));
}

//==============================================================================
void PythonShell::resized()
{
    Component* comps[] = {std::addressof(outputDisplay), std::addressof(resizer), std::addressof(editor) };
    layout.layOutComponents(comps, 3, 0, 0, getWidth(), getHeight(), true, true);
}

//==============================================================================
bool PythonShell::keyPressed (const juce::KeyPress &key, juce::Component *originatingComponent)
{
    if (originatingComponent == std::addressof(editor)
        && key.isKeyCode(juce::KeyPress::returnKey)
        && key.getModifiers().isCtrlDown()) {

        // Run the python code from the code editor
        std::string codeToRun;
        bool removeCode = true;
        if (editor.isHighlightActive()) {
            auto highlightedRegion = editor.getHighlightedRegion();
            codeToRun = editor.getTextInRange(highlightedRegion).toStdString();
            removeCode = false;
        }
        else {
            codeToRun = codeDocument.getAllContent().toStdString();
        }
        try {
            py::exec(codeToRun);
        } catch (py::error_already_set &e) {
            auto tb = e.trace();
            auto traceback = py::module_::import("traceback");
            auto sys = py::module_::import("sys");
            traceback.attr("print_exception")(e.type(), e.value(), tb, py::none(), sys.attr("stdout"));
            removeCode = false;
        }
        if (removeCode) {
            codeDocument.replaceAllContent("");
        }
        return true;
    }
    return false;
}
