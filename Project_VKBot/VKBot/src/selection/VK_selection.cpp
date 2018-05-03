#include "selection/VK_selection.h"

namespace {
    const char SELECTION_EXTENSION[4] = "vks";
}

vk_selection::Selection::Selection(std::string name) : isInverted_(false), size_(0), name_("") {
    std::stringstream fileName;
    fileName << name << "." << SELECTION_EXTENSION;
    name_ = fileName.str();
    std::FILE *file = std::fopen(name_.c_str(), "wb");
    if (isInverted_) {
        char oneChar = '1';
        std::fwrite(&oneChar, sizeof(char), 1, file);
    } else {
        char zeroChar = '0';
        std::fwrite(&zeroChar, sizeof(char), 1, file);
    }
    std::fwrite(&size_, sizeof(size_t), 1, file);
    std::fclose(file);
}

vk_selection::Selection::Selection(const vk_selection::Selection &other, std::string name) : isInverted_(other.isInverted_), size_(other.size_), name_("") {
    std::stringstream fileName;
    fileName << name << "." << SELECTION_EXTENSION;
    name_ = fileName.str();
    vk_selection::Selection::inTwoFiles_(other, "rb", *this, "wb", [](std::FILE *source, std::FILE *target){
        size_t B_SIZE = 256;
        char buffer[B_SIZE];
        size_t s = std::fread(buffer, sizeof(char), B_SIZE, source);
        while (s) {
            std::fwrite(buffer, sizeof(char), s, target);
            s = std::fread(buffer, sizeof(char), B_SIZE, source);
        }
    });
}

vk_selection::Selection::~Selection()
{
    if (std::remove(name_.c_str()) != 0) {
        std::cerr << "Error: Unable to remove " << name_ << " with size of ~" << (size_ * 5 / 1000) << "MB" << std::endl;
    }
}


vk_selection::Selection vk_selection::Selection::operator&&(const Selection& other) const {
    // TODO implementation
}
vk_selection::Selection vk_selection::Selection::operator||(const Selection& other) const {
    // TODO implementation
}
vk_selection::Selection vk_selection::Selection::operator!() {
    // TODO implementation
}

void vk_selection::Selection::updateInfo_ () {
    std::FILE *file = std::fopen(name_.c_str(), "ab");
    std::fseek(file, 0, SEEK_SET); // Moving to the beginning of file
    if (isInverted_) {
        char oneChar = '1';
        std::fwrite(&oneChar, sizeof(char), 1, file);
    } else {
        char zeroChar = '0';
        std::fwrite(&zeroChar, sizeof(char), 1, file);
    }
    std::fwrite(&size_, sizeof(size_t), 1, file);
    std::fclose(file);
}

void vk_selection::Selection::saveAs (std::string name) {
    std::stringstream fileName;
    fileName << name << ".txt";
    std::FILE *file = std::fopen(fileName.str().c_str(), "w");
    std::fprintf(file, "%s", isInverted_ ? "Inverted\n" : "Not inverted\n");
    std::fprintf(file, "%s%lu%s", "Size of selection: ", size_, "\n");
    inFile_("rb", [this, unitTypeNames, file](std::FILE *b_file){
        std::fseek(b_file, sizeof(char) + sizeof(size_t), SEEK_SET); // Moving to the beginning of data
        for (size_t i = 0; i < size_; i++) {
            char u; // unit type
            std::fread(&u, sizeof(char), 1, b_file);
            std::fprintf(file, "%s", vk_selection::unitTypeNames[((int) u) - ((int) '0')].c_str()); // Using '0' as first element in enum

            vkid_t id; // unit id
            std::fread(&id, sizeof(vkid_t), 1, b_file);
            std::fprintf(file, "%c%lu%s", ' ', id, "\n");
        }
    });

    std::fclose(file);
}

bool vk_selection::Selection::isInverted() const {
    return isInverted_;
}
size_t vk_selection::Selection::size() const {
    return size_;
}

std::ostream &vk_selection::operator<< (std::ostream &os, const vk_selection::Unit &unit) {
    return os << vk_selection::unitTypeNames[(int) unit.type() - (int) '0'] << " " << unit.id() << " " << unit.customId();
}
