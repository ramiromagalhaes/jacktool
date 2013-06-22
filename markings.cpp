#include "markings.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>

Markings::Markings()
{
    empty = new std::vector<Rectangle>();
}

Markings::Markings(const std::string base_directory_)
{
    changeBaseDirectory(base_directory_);
}

void Markings::changeBaseDirectory(const std::string base_directory_)
{
    base_directory = base_directory_;
    is_dirty = false;

    for(std::map<std::string, std::vector<Rectangle>*>::iterator it = exclusions.begin(); it != exclusions.end(); ) {
        std::map<std::string, std::vector<Rectangle> *>::value_type v = *it;
        delete v.second;
    }

    exclusions.clear();
    load();
}

void Markings::set(const std::string image, std::vector<Rectangle> *markings)
{
    std::map<std::string, std::vector<Rectangle> *>::iterator it = exclusions.find(image);

    if ( it == exclusions.end() )
    {
        exclusions.insert(std::map<std::string, std::vector<Rectangle> *>::value_type(image, markings));
    }
    else
    {
        exclusions[image] = markings;
    }

    is_dirty = true;
}

const std::vector<Rectangle> * Markings::get(const std::string &image)
{
    std::map<std::string, std::vector<Rectangle> *>::iterator it = exclusions.find(image);
    if ( it != exclusions.end() )
    {
        std::map<std::string, std::vector<Rectangle> *>::value_type v = *it;
        return v.second;
    }

    return empty;
}

void Markings::remove(const std::string & image)
{
    std::map<std::string, std::vector<Rectangle> *>::iterator it = exclusions.find(image);
    if ( it != exclusions.end() )
    {
        exclusions.erase(it);
    }

    is_dirty = true;
}

void Markings::save()
{
    boost::filesystem::path archive( base_directory );
    archive = archive / ".jacktool.data";

    if ( !boost::filesystem::exists(archive) )
    {
        create();
    }

    boost::filesystem::ofstream ofs( archive );
    boost::archive::text_oarchive oa(ofs);
    oa << *this;
    ofs.close();

    is_dirty = false;
}

bool Markings::isDirty()
{
    return is_dirty;
}

bool Markings::load()
{
    boost::filesystem::path archive( base_directory );
    archive = archive / ".jacktool.data";

    if ( !boost::filesystem::exists(archive) )
    {
        return false;
    }

    boost::filesystem::ifstream ifs(archive);
    boost::archive::text_iarchive ia(ifs);
    ia >> *this;
    ifs.close();

    return true;
}

void Markings::create()
{
    boost::filesystem::path archive( base_directory );
    archive = archive / ".jacktool.data";

    boost::filesystem::ofstream ofs( archive );
    ofs.close();
}

template<class Archive>
void Markings::serialize(Archive &ar, const unsigned int)
{
    ar & base_directory;
    ar & exclusions;
}
