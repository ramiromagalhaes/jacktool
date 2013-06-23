#ifndef LABELFILE_H
#define LABELFILE_H

#include <string>
#include <vector>
#include <map>
#include "Rectangle.h"
#include "PatchExtractorConfiguration.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>


class Markings
{
public:
    Markings();
    Markings(const std::string base_directory_);
    void changeBaseDirectory(const std::string base_directory_);
    void set(const std::string image, std::vector<Rectangle> * markings);
    const std::vector<Rectangle> * get(const std::string & image);
    void remove(const std::string & image);
    void save();
    void processAll(const PatchExtractorConfiguration & cfg);

    bool isDirty();

private:
    bool load();
    void create();

    friend class boost::serialization::access;

    bool is_dirty;

    std::string base_directory;
    std::map<std::string, std::vector<Rectangle>*> exclusions;

    std::vector<Rectangle> * empty;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int);
};

#endif // LABELFILE_H
