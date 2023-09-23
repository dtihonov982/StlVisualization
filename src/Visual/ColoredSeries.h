#include "Series.h"

// Add a color for each element from Series
class ColoredSeries: public Series {
public:
    void setDefaultColor(const SDL_Color& color) { defaultColor_ = color; }

    void setColorsToDefault() {
        std::fill(colors_.begin(), colors_.end(), defaultColor_);
    }

    void setColorsToDefault(size_t pos) {
        if (pos >= colors_.size()) 
            throw Exception("Invalid position ", pos);
        colors_[pos] = defaultColor_;
    }

    template<typename It>
    void load(It begin, It end) {
        if (end < begin)
            throw Exception("Argument invalid.");
        Series::load(begin, end);
        updateColors(begin, end);
    }

    template<typename It>
    void load(It begin, It end, typename It::value_type maxValue) {
        if (end < begin)
            throw Exception("Argument invalid.");
        Series::load(begin, end, maxValue);
        updateColors(begin, end);
    }

    template<typename It>
    void updateColors(It begin, It end) {
        if (end < begin)
            throw Exception("Argument invalid.");
        colors_.resize(end - begin);
        setColorsToDefault();
    }

    void setElementColor(size_t pos, const SDL_Color& color) {
        if (pos >= colors_.size()) 
            throw Exception("Invalid position ", pos);
        colors_[pos] = color;
    }

    const std::vector<SDL_Color>& getColors() const { return colors_; }
private:
    std::vector<SDL_Color> colors_;
    SDL_Color defaultColor_ {0xff, 0xff, 0xff, 0xff};
};
