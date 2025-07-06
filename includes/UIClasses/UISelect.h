#pragma once

#include "UIBaseElement.h"
#include <string>
#include <vector>

class UISelect : public BaseUIElement {
public:
    UISelect(std::string&, std::vector<std::string>&);
    virtual void render() override;

    void onChange(bool);
    
    bool isChanged();
    
    std::string getSelectedOption();
    int getSelectedIndex();
    void setSelectedIndex(int);

private:
    std::string m_label;
    int m_selectedIndex;
    std::vector<std::string> m_options;
};