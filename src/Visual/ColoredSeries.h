#include "Series.h"

// Add a color for each element from Series
class ColoredSeries: public Series {
public:
    void setDefaultColor(const SDL_Color& color) { defaultColor_ = color; }

    void setColorsToDefault() {
        std::fill(colors_.begin(), colors_.end(), defaultColor_);
    }

    template<typename It>
    void load(It begin, It end) {
        if (end < begin)
            throw Exception("Argument invalid.");
        Series::load(begin, end);
        colors_.resize(end - begin);
        setColorsToDefault();
    }

    void setElementColor(size_t pos, const SDL_Color& color) {
        if (pos < 0 || pos >= colors_.size()) 
            throw Exception("Invalid position ", pos);
        colors_[pos] = color;
    }

    const std::vector<SDL_Color>& getColors() const { return colors_; }
private:
    std::vector<SDL_Color> colors_;
    SDL_Color defaultColor_ {0xff, 0xff, 0xff, 0xff};
};
