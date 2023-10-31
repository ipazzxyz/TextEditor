#include "../include/texteditor.h"
#include "ui_texteditor.h"
#include <QFileDialog>

TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TextEditor) {
  this->ui->setupUi(this);
  this->update();
}

TextEditor::~TextEditor() { delete ui; }

void TextEditor::update() {
  this->ui->data->setText(QString::fromStdWString(this->buffer.get()));
  this->ui->lines_cnt->setText(QString::number(this->buffer.size()));
}

void TextEditor::status(size_t status) {
  if (status == 0) {
    this->ui->status->setText("");
  } else if (status == 1) {
    this->ui->status->setText("Error: Out of range");
  } else if (status == 2) {
    this->ui->status->setText("Error: No suitable options");
  }
}

void TextEditor::on_insert_clicked() {
  if (this->ui->m_active->isChecked() && this->ui->input2_active->isChecked()) {
    this->status(2);
  } else if (this->ui->m_active->isChecked()) {
    this->insert(this->ui->n->text().toUInt(), this->ui->m->text().toUInt(),
                 this->ui->input->text().toStdWString());
  } else if (this->ui->input2_active->isChecked()) {
    this->status(2);
  } else {
    this->insert(this->ui->n->text().toUInt(),
                 this->ui->input->text().toStdWString());
  }

  this->update();
}

void TextEditor::on_replace_clicked() {
  if (this->ui->m_active->isChecked() && this->ui->input2_active->isChecked()) {
    this->replace(this->ui->n->text().toUInt(), this->ui->m->text().toUInt(),
                  this->ui->input->text().toStdWString(),
                  this->ui->input2->text().toStdWString());
  } else if (this->ui->m_active->isChecked()) {
    this->status(2);
  } else if (this->ui->input2_active->isChecked()) {
    this->replace(1, this->buffer.size(),
                  this->ui->input->text().toStdWString(),
                  this->ui->input2->text().toStdWString());
  } else {
    this->status(2);
  }

  this->update();
}

void TextEditor::on_special_clicked() {
  if (this->ui->special_select->currentIndex() == 0) {
    if (this->ui->m_active->isChecked() &&
        this->ui->input2_active->isChecked()) {
      this->status(2);
    } else if (this->ui->m_active->isChecked()) {
      this->eraseLeadingZeros(this->ui->n->text().toUInt(),
                              this->ui->m->text().toUInt());
    } else if (this->ui->input2_active->isChecked()) {
      this->status(2);
    } else {
      this->eraseLeadingZeros(1, this->buffer.size());
    }
  } else if (this->ui->special_select->currentIndex() == 1) {
    if (this->ui->m_active->isChecked() &&
        this->ui->input2_active->isChecked()) {
      this->status(2);
    } else if (this->ui->m_active->isChecked()) {
      this->eraseNonDecreasing(this->ui->n->text().toUInt(),
                               this->ui->m->text().toUInt());
    } else if (this->ui->input2_active->isChecked()) {
      this->status(2);
    } else {
      this->eraseNonDecreasing(1, this->buffer.size());
    }
  } else if (this->ui->special_select->currentIndex() == 2) {
    if (this->ui->m_active->isChecked() &&
        this->ui->input2_active->isChecked()) {
      this->status(2);
    } else if (this->ui->m_active->isChecked()) {
      this->reduceRepetitiveAsterisk(this->ui->n->text().toUInt(),
                                     this->ui->m->text().toUInt());
    } else if (this->ui->input2_active->isChecked()) {
      this->status(2);
    } else {
      this->reduceRepetitiveAsterisk(1, this->buffer.size());
    }
  } else if (this->ui->special_select->currentIndex() == 3) {
    if (this->ui->m_active->isChecked() &&
        this->ui->input2_active->isChecked()) {
      this->status(2);
    } else if (this->ui->m_active->isChecked()) {
      this->eraseCurlyBrackets(this->ui->n->text().toUInt(),
                               this->ui->m->text().toUInt());
    } else if (this->ui->input2_active->isChecked()) {
      this->status(2);
    } else {
      this->eraseCurlyBrackets(1, this->buffer.size());
    }
  }

  this->update();
}

void TextEditor::on_erase_clicked() {
  if (this->ui->m_active->isChecked() && this->ui->input2_active->isChecked()) {
    this->status(2);
  } else if (this->ui->m_active->isChecked()) {
    this->erase(this->ui->n->text().toUInt(), this->ui->m->text().toUInt());
  } else if (this->ui->input2_active->isChecked()) {
    this->status(2);
  } else {
    this->erase(this->ui->n->text().toUInt(), this->ui->n->text().toUInt());
  }

  this->update();
}

void TextEditor::on_save_clicked() { this->save(); }

void TextEditor::on_load_clicked() { this->load(); }

void TextEditor::insert(const size_t &N, const std::wstring &input) {
  this->status(0);

  try {
    this->buffer.insert(N, input);
  } catch (std::out_of_range) {
    this->status(1);
  }
}

void TextEditor::insert(const size_t &N, const size_t &M,
                        const std::wstring &input) {
  this->status(0);

  try {
    this->buffer.insert(N - 1, M, input);
  } catch (std::out_of_range) {
    this->status(1);
  }
}

void TextEditor::erase(const size_t &N, const size_t &M) {
  this->status(0);

  try {
    this->buffer.erase(N - 1, M - 1);
  } catch (std::out_of_range) {
    this->status(1);
  }
}

void TextEditor::replace(const size_t &N, const size_t &M,
                         const std::wstring &input,
                         const std::wstring &input2) {
  this->status(0);

  try {
    this->buffer.replace(N - 1, M - 1, input, input2);
  } catch (std::out_of_range) {
    this->status(1);
  }
}

void TextEditor::save() {
  QFile file(QFileDialog::getSaveFileName(this, tr("Open file"),
                                          tr("Text Files (*.txt)")));
  if (file.open(QIODevice::WriteOnly)) {
    QTextStream out(&file);
    out << QString::fromStdWString(this->buffer.get());
    file.close();
  }
}

void TextEditor::load() {
  this->buffer.clear();

  QFile file(QFileDialog::getSaveFileName(this, tr("Open file"),
                                          tr("Text Files (*.txt)")));
  if (file.open(QIODevice::ReadOnly)) {
    QTextStream in(&file);
    std::wstring s;
    std::vector<std::wstring> v;
    while (s = in.readLine().toStdWString(), !s.empty()) {
      v.push_back(s);
    }
    this->buffer.insert(0, v);
  }

  this->update();
}

void TextEditor::eraseLeadingZeros(const size_t &N, const size_t &M) {
  this->status(0);

  try {
    this->buffer.eraseLeadingZeros(N - 1, M - 1);
  } catch (std::out_of_range) {
    this->status(1);
  }
}
void TextEditor::eraseNonDecreasing(const size_t &N, const size_t &M) {
  this->status(0);

  try {
    this->buffer.eraseNonDecreasing(N - 1, M - 1);
  } catch (std::out_of_range) {
    this->status(1);
  }
}
void TextEditor::reduceRepetitiveAsterisk(const size_t &N, const size_t &M) {
  this->status(0);

  try {
    this->buffer.reduceRepetitiveAsterisk(N - 1, M - 1);
  } catch (std::out_of_range) {
    this->status(1);
  }
}
void TextEditor::eraseCurlyBrackets(const size_t &N, const size_t &M) {
  this->status(0);

  try {
    this->buffer.eraseCurlyBrackets(N - 1, M - 1);
  } catch (std::out_of_range) {
    this->status(1);
  }
}
