#ifndef __TOOL_IMPORT_HPP__
#define __TOOL_IMPORT_HPP__

// Include Json
#include <nlohmann/json.hpp>

// Include SFML
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>

// Include Libraries
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <functional>
#include <cmath>
#include <fstream>
#include <sstream>
#include <random>
#include <thread>
#include <mutex>

namespace std {
    wstring translate_to_wstring(std::string text) { 
        return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(text); 
    }
}

// Include NG Tools Graphics
#include <NG_Tools/tool/graphic/surface.hpp>

// Include NG Tools System
#include <NG_Tools/tool/object/sample.hpp>
#include <NG_Tools/tool/object/frame.hpp>
#include <NG_Tools/tool/system/window.hpp>

// Include NG Tools Objects
#include <NG_Tools/tool/object/button.hpp>
#include <NG_Tools/tool/object/sensor.hpp>
#include <NG_Tools/tool/object/lever.hpp>
#include <NG_tools/tool/object/spinbox.hpp>

#include <NG_Tools/tool/object/list.hpp>
#include <NG_Tools/tool/object/checkradio.hpp>
#include <NG_Tools/tool/object/checkbox.hpp>

#include <NG_Tools/tool/object/label.hpp>
#include <NG_Tools/tool/object/entry.hpp>
#include <NG_Tools/tool/object/image.hpp>

#include <NG_Tools/tool/object/scrollbar.hpp>
#include <NG_Tools/tool/object/slider.hpp>
#include <NG_Tools/tool/object/bucket.hpp>

#include <NG_Tools/tool/object/steper.hpp>
#include <NG_Tools/tool/object/scale.hpp>

/*
    - Cập nhật function
    - Cập nhật tinh chỉnh nâng cao
    - Cập nhật tên hàm, tên biến
    - Cập nhật thêm chữ và hình cho 3 dạng button
    - Cập nhật tùy chỉnh chữ cho text
*/

#endif
