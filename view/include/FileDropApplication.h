#pragma once

#include "../../model/include/Session.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WFileDropWidget.h>

#include <map>
#include <vector>

namespace Wt {
  class WText;
}

namespace Fxt {

  class FileDropApplication : public Wt::WContainerWidget
  {
  public:
    FileDropApplication(Session& session, const std::string uploadPath);
    std::vector<std::string> uploadedFiles() const;
    void cancelUpload();

  private:
    Session& session_;
    
    Wt::WText *log_;
    Wt::WFileDropWidget *drop_;
    Wt::WProgressBar *bar_;
    int nbUploads_;
    std::string saveName, uploadPath_;

    std::vector<std::string> uploadedfiles_;

    std::map<Wt::WFileDropWidget::File*, Wt::WContainerWidget*> icons_;

    void handleDrop(std::vector<Wt::WFileDropWidget::File *> files);
    void tooLarge(Wt::WFileDropWidget::File *file, ::uint64_t);
    void failed(Wt::WFileDropWidget::File *file);
    void saveFile(Wt::WFileDropWidget::File *file);
    void updateProgressListener();

    void showProgress(::uint64_t current, ::uint64_t total);
  };

}
