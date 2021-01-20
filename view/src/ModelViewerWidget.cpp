#include "ModelViewerWidget.h"
#include "ModelViewerControls.h"
#include "PaintWidget.h"
#include "ModelInfoPanels.h"

#include <Wt/WImage.h>
#include <Wt/WGridLayout.h>
#include <Wt/WApplication.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WEnvironment.h>

#include <Wt/WPaintDevice.h>
#include <Wt/WPaintedWidget.h>
#include <Wt/WPainter.h>

#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WDialog.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTextArea.h>
#include <Wt/WFileDropWidget.h>
#include <Wt/WComboBox.h>
#include <Wt/WProgressBar.h>
#include <Wt/WTimer.h>
#include <Wt/WTable.h>
#include <Wt/WMessageBox.h>

#include <thread>
#include <future>
#include <memory>
#include <map>

#include "../include/BreadCrumbWidget.h"
#include "../include/FeatureDialog.h"

#include "../../dbdao/include/ModelFileDao.h"

#include "../../libfxtract/include/Computation.h"

#include "../../libfxtract/include/obj-file-reader/readObj.h"

class MyPaintedWidget : public Wt::WPaintedWidget
{
public:
    MyPaintedWidget()
        : WPaintedWidget(), end_(100)
    {
	resize(200, 60);   // Provide a default size.
    }

    void setEnd(int end) {
	end_ = end;
	update();          // Trigger a repaint.
    }

protected:
    void paintEvent(Wt::WPaintDevice *paintDevice) {
        Wt::WPainter painter(paintDevice);
        painter.setBrush(Wt::WBrush(Wt::WColor(Wt::StandardColor::Blue)));
        painter.drawRect(0, 0, end_, 50);
    }

private:
    int end_;
};

namespace {

    /* 
     *  Map for storing obj data for all the project's model files as they are clicked
     *  Avoid multiple calls to the extractObjData() for the same data.
    */
    std::map<std::string, std::vector<float>> objDataMap;

    const char *fragmentShaderSrc =
    "#ifdef GL_ES\n"
    "precision highp float;\n"
    "#endif\n"
    "\n"
    "varying vec3 vLightWeighting;\n"
    "\n"
    "void main(void) {\n"
    "  vec4 matColor = vec4(0.278, 0.768, 0.353, 1.0);\n"
    "  gl_FragColor = vec4(matColor.rgb * vLightWeighting, matColor.a);\n"
    "}\n";

    const char *vertexShaderSrc =
    "attribute vec3 aVertexPosition;\n"
    "attribute vec3 aVertexNormal;\n"
    "\n"
    "uniform mat4 uMVMatrix; // [M]odel[V]iew matrix\n"
    "uniform mat4 uCMatrix;  // Client-side manipulated [C]amera matrix\n"
    "uniform mat4 uPMatrix;  // Perspective [P]rojection matrix\n"
    "uniform mat4 uNMatrix;  // [N]ormal transformation\n"
    "// uNMatrix is the transpose of the inverse of uCMatrix * uMVMatrix\n"
    "\n"
    "varying vec3 vLightWeighting;\n"
    "\n"
    "void main(void) {\n"
    "  // Calculate the position of this vertex\n"
    "  gl_Position = uPMatrix * uCMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);\n"
    "\n"
    "  // Phong shading\n"
    "  vec3 transformedNormal = normalize((uNMatrix * vec4(normalize(aVertexNormal), 0)).xyz);\n"
    "  vec3 lightingDirection = normalize(vec3(1, 1, 1));\n"
    "  float directionalLightWeighting = max(dot(transformedNormal, lightingDirection), 0.0);\n"
    "  vec3 uAmbientLightColor = vec3(0.2, 0.2, 0.2);\n"
    "  vec3 uDirectionalColor = vec3(0.8, 0.8, 0.8);\n"
    "  vLightWeighting = uAmbientLightColor + uDirectionalColor * directionalLightWeighting;\n"
    "}\n";
}

ModelViewerWidget::ModelViewerWidget(Session& session, dbo::ptr<Project> project, const std::string uploadPath)
    :WContainerWidget(),
    session_(session),
    project_(project),
    paintWidget_(0),
    uploadPath_(uploadPath)
{
    setLayoutSizeAware(true);
    setMargin(0, Wt::AllSides);
    setInline(false);
    addStyleClass("MVW", true);

    objDataMap.clear();

    auto vBox = setLayout(Wt::cpp14::make_unique<Wt::WVBoxLayout>());
    vBox->setContentsMargins(0, 0, 0, 0);

    breadCrumb_ = vBox->addWidget(cpp14::make_unique<BreadCrumbWidget>(session_, project_->title.toUTF8()));  

    hbox_ = vBox->addLayout(Wt::cpp14::make_unique<Wt::WGridLayout>());     

    auto m = Wt::cpp14::make_unique<ModelInfoPanels>(session_, project_);
    modelInfoPanels_ = hbox_->addWidget(std::move(m), 0, 0);
    modelInfoPanels_->setMargin(10, Wt::Side::Left | Wt::Side::Top);

    auto controls_ = Wt::cpp14::make_unique<ModelViewerControls>(session_, uploadPath_,  project);
    controls_->addStyleClass("controls", true);

    hbox_->addWidget(std::move(controls_), 0, 2);
    hbox_->setColumnStretch(1, 1); 

    paintWidgetContainer_ = hbox_->addWidget(std::move(Wt::cpp14::make_unique<WContainerWidget>()), 0, 1);
    paintWidgetContainer_->resize(1200, 800);

    session_.modelFileClicked().connect(this, &ModelViewerWidget::updateShaders);
    session_.modelFileDeleted().connect(this, &ModelViewerWidget::clearWebGL);

    session_.modelFeaturesExtracted().connect(this, &ModelViewerWidget::setBendDirection);
}

void ModelViewerWidget::setBendDirection(dbo::ptr<ModelFile> modelFile)
{
    auto dialog = addChild(Wt::cpp14::make_unique<FeatureDialog>(session_, "Model File : " + modelFile->modelFile.toUTF8(), modelFile));
    dialog->setWidth(900);
    dialog->show();
}

ModelViewerWidget::~ModelViewerWidget() {}

void ModelViewerWidget::clearWebGL()
{
    paintWidgetContainer_->clear();
}

std::vector<float> extractObjData(std::string objFile)
{
    auto data = std::make_unique<std::vector<float>>();
    auto pObjReader = cpp14::make_unique<Converter::ObjReader>();
    pObjReader->objParser(objFile, data);

    return *data;
}

void ModelViewerWidget::updateShaders(dbo::ptr<ModelFile> modelFile)
{
    std::vector<float> data;
    std::string objFile = modelFile->modelFileDir.toUTF8() + modelFile->modelObjFile.toUTF8();

    if (objDataMap.find(objFile) != objDataMap.end()) {
        data = objDataMap[objFile];
    } else {
        /**
         * Asynchronously extract obj data from the file if it's not in the objDataMap already.
         * Add the data into the map afterwards.
         */
        std::future<std::vector<float> > futureData = std::async(std::launch::async, extractObjData, objFile);
        data = futureData.get();
        objDataMap[objFile] = data;
    }

    displayWebGL(data);
}

void ModelViewerWidget::displayWebGL(std::vector<float> data)
{
    Http::ParameterValues pv = Wt::WApplication::instance()->environment().getParameterValues("binaryBuffers");
    bool useBinaryBuffers = false;

    if (!pv.empty()) {
        useBinaryBuffers = true;
    }

    if(paintWidget_)
        paintWidget_->removeFromParent();
    
    paintWidgetContainer_->clear();
    paintWidget_ = paintWidgetContainer_->addWidget(cpp14::make_unique<PaintWidget>(data, useBinaryBuffers));
    paintWidget_->setMinimumSize(700, 300);
    paintWidget_->setStyleClass("centered");
    paintWidget_->resize(1200, 800);
    paintWidget_->setShaders(vertexShaderSrc, fragmentShaderSrc);
    paintWidget_->setAlternativeContent(Wt::cpp14::make_unique<Wt::WImage>("nowebgl.png"));

    paintWidget_->addChild(Wt::cpp14::make_unique<MyPaintedWidget>());
}