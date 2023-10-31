#pragma once
#include "buffer.h"
#include <QFileDialog>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class TextEditor;
}
QT_END_NAMESPACE

class TextEditor : public QMainWindow {
  Q_OBJECT

public:
  TextEditor(QWidget *parent = nullptr);
  ~TextEditor();

private slots:
  void on_insert_clicked();
  void on_erase_clicked();
  void on_replace_clicked();
  void on_special_clicked();
  void on_save_clicked();
  void on_load_clicked();

private:
  Ui::TextEditor *ui;
  Buffer buffer;

  void insert(const size_t &, const std::wstring &);
  void insert(const size_t &, const size_t &, const std::wstring &);

  void erase(const size_t &, const size_t &);

  void replace(const size_t &, const size_t &, const std::wstring &,
               const std::wstring &);

  void eraseLeadingZeros(const size_t &, const size_t &);
  void eraseNonDecreasing(const size_t &, const size_t &);
  void reduceRepetitiveAsterisk(const size_t &, const size_t &);
  void eraseCurlyBrackets(const size_t &, const size_t &);

  void save();
  void load();

  void update();

  void status(size_t status);
};
