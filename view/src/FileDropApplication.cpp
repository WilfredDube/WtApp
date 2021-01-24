/*
* Copyright (C) 2016 Emweb bvba, Herent, Belgium.
*
* See the LICENSE file for terms of use.
*/

#include "../include/FileDropApplication.h"

#include <Wt/WText.h>
#include <Wt/WProgressBar.h>
#include "Wt/Utils.h"

#include <fstream>

#include "../../model/include/Session.h"
#include "../../dbdao/include/UserDao.h"

namespace Fxt {

  namespace {
    const std::string TEMP_FOLDER = "/temp/";
    std::string UPLOAD_FOLDER;
    static const int MAX_FILES = 36;
  }

  std::vector<std::string> FileDropApplication::uploadedFiles() const { return uploadedfiles_; }

  FileDropApplication::FileDropApplication(Session& session, const std::string uploadPath)
  : WContainerWidget(),
  session_(session),
  uploadPath_(uploadPath),
  nbUploads_(0)
  {
    UPLOAD_FOLDER = uploadPath_ + "/" + session_.user()->name.toUTF8() + TEMP_FOLDER;

    addWidget(Wt::cpp14::make_unique<Wt::WText>("<B>Try dropping a file in the widget below</B>"));

    drop_ = addWidget(Wt::cpp14::make_unique<Wt::WFileDropWidget>());

    drop_->setDropIndicationEnabled(true);
    drop_->setWidth(570);

    drop_->drop().connect(this, &FileDropApplication::handleDrop);
    drop_->newUpload().connect(this,&FileDropApplication::updateProgressListener);
    drop_->uploaded().connect(this, &FileDropApplication::saveFile);
    drop_->uploadFailed().connect(this, &FileDropApplication::failed);
    drop_->tooLarge().connect(this, &FileDropApplication::tooLarge);

    bar_ = addWidget(Wt::cpp14::make_unique<Wt::WProgressBar>());
    bar_->setWidth(570);
    bar_->setHeight(8);
    bar_->setValue(0);
    bar_->setRange(0, 10);

    log_ = addWidget(Wt::cpp14::make_unique<Wt::WText>());
    log_->setInline(false);
    log_->setTextFormat(Wt::TextFormat::XHTML);

    // Wt::WPushButton *abort = addWidget(Wt::cpp14::make_unique<Wt::WPushButton>("Abort current upload"));
    // abort->clicked().connect(this, &FileDropApplication::cancelUpload);
  }

  void FileDropApplication::handleDrop(std::vector<Wt::WFileDropWidget::File *> files)
  {
  for (unsigned i=0; i < files.size(); i++) {
    Wt::WFileDropWidget::File *file = files[i];
    if (nbUploads_ >= MAX_FILES) {
      drop_->cancelUpload(file);
      continue;
    }

    uploadedfiles_.push_back(file->clientFileName());

    auto block = drop_->addNew<Wt::WContainerWidget>();
    block->setToolTip(file->clientFileName() + " [" + file->mimeType() + "]");
    block->addStyleClass("upload-block");

    icons_[file] = block;
    nbUploads_++;
  }
  
  if (nbUploads_ >= MAX_FILES) {
    log_->setText("That's enough ...");
    drop_->setAcceptDrops(false);
  }
}

void FileDropApplication::cancelUpload()
{
  if (drop_->uploads().size() == drop_->currentIndex())
    return;
  
  Wt::WFileDropWidget::File *currentFile = drop_->uploads()[drop_->currentIndex()];
  drop_->cancelUpload(currentFile);
  icons_[currentFile]->addStyleClass("cancelled");
}

void FileDropApplication::tooLarge(Wt::WFileDropWidget::File *file, ::uint64_t)
{
  icons_[file]->addStyleClass("invalid");
  
  log_->setText("File too large: " + file->clientFileName());
}

void FileDropApplication::failed(Wt::WFileDropWidget::File *file)
{
  icons_[file]->addStyleClass("invalid");
  
  log_->setText("Upload failed: " + file->clientFileName());
}

void FileDropApplication::saveFile(Wt::WFileDropWidget::File *file)
{
  std::string spool = file->uploadedFile().spoolFileName();
  std::ifstream src(spool.c_str(), std::ios::binary);

  std::string saveName = UPLOAD_FOLDER + file->clientFileName();
  
  std::ofstream dest(saveName.c_str(), std::ios::binary);
  if (dest.fail()) {
    std::cerr << "**** ERROR: The output file could not be opened"
  	      << std::endl;
    return;
  }
  
  dest << src.rdbuf();

  if (icons_.find(file) != icons_.end()) {
    icons_[file]->addStyleClass("ready");
    drop_->remove(file);
  }
}

  void FileDropApplication::updateProgressListener()
  {
    // if there is a next file listen for progress
    bar_->setValue(0);
    if ((unsigned)drop_->currentIndex() < drop_->uploads().size()) {
      Wt::WFileDropWidget::File *file = drop_->uploads()[drop_->currentIndex()];
      file->dataReceived().connect(this, &FileDropApplication::showProgress);
      std::string fileName = Wt::Utils::htmlEncode(file->clientFileName());
      log_->setText("uploading file &quot;" + fileName + "&quot;");
    }
  }

  void FileDropApplication::showProgress(::uint64_t current, ::uint64_t total)
  {
    Wt::WFileDropWidget::File *file = drop_->uploads()[drop_->currentIndex()];
    std::string c = std::to_string(current/1024);
    std::string t = std::to_string(total/1024);
    std::string fileName = Wt::Utils::htmlEncode(file->clientFileName());
    bar_->setValue(bar_->value() + (current/total * 10));
    log_->setText("Uploading file <i>&quot;" + fileName + "&quot;</i>"
    + " (" + c + "kB" + " out of " + t + "kB)");
  }

}
