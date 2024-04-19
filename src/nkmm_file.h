// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_FILE_H
#define NKMM_FILE_H

namespace nkmm {

//class String;

class File {
 public:
  File();
  File(const nkmm::string& fileName, bool writeMode = false);
  virtual ~File();

  int open(const nkmm::string& fileName, bool writeMode = false);
  void close();

  uint32_t getLastModified();  // 最終更新日の取得
  int getSize() const { return size_; }

  bool isEof() const;

  int getChar();
  int putChar(int ch);

  void unGetChar(int ch);

  int read(void* buf, int size = -1);
  int allocRead(void** buf);

  int write(void* buf, int size);

 private:
  uint32_t fp_;  //!< ファイルポインタ
  int size_;     //!< サイズ
};

}  //nkmm

#endif  //NKMM_FILE_H
