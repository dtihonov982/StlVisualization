#include "Visual/Playlist.cpp"

int main() {
    auto entry = Playlist::Entry::loadFromString("10,abc.txt,def.txt");
    Playlist playlist = Playlist::loadFromFile("test.list");
    return 0;
}
