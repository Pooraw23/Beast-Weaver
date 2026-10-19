#include "TerminalHUD.h"

// Define static constants
const string TerminalHUD::RESET   = "\033[0m";
const string TerminalHUD::BOLD    = "\033[1m";
const string TerminalHUD::RED     = "\033[1;31m";
const string TerminalHUD::GREEN   = "\033[1;32m";
const string TerminalHUD::YELLOW  = "\033[1;33m";
const string TerminalHUD::BLUE    = "\033[1;34m";
const string TerminalHUD::MAGENTA = "\033[1;35m";
const string TerminalHUD::CYAN    = "\033[1;36m";
const string TerminalHUD::WHITE   = "\033[1;37m";

string TerminalHUD::colorize(const string& text, const string& color) {
    return color + text + RESET;
}

// Direction 4: Color-coded health bar based on HP percentage
string TerminalHUD::renderColorHpBar(int current, int max, int length) {
    if (max <= 0) max = 1;
    if (current < 0) current = 0;
    if (current > max) current = max;

    float ratio = static_cast<float>(current) / max;
    int filled = static_cast<int>(ratio * length);
    if (filled == 0 && current > 0) filled = 1;

    // Determine color: Green (>50%), Yellow (25-50%), Red (<25%)
    string barColor = GREEN;
    if (ratio <= 0.25f) barColor = RED;
    else if (ratio <= 0.50f) barColor = YELLOW;

    string bar = "[";
    bar += barColor;
    for (int i = 0; i < filled; i++) bar += "█";
    bar += RESET;
    for (int i = filled; i < length; i++) bar += "░";
    bar += "] ";

    return bar;
}

string TerminalHUD::getElementalBadge(int type) {
    if (type == 0) return RED + "🔥 FIRE" + RESET;
    if (type == 1) return CYAN + "💧 WATER" + RESET;
    return GREEN + "🌿 GRASS" + RESET;
}

string TerminalHUD::getStatusBadge(int status) {
    switch (status) {
        case 1: return MAGENTA + "☠️ POISON" + RESET;
        case 2: return RED + "🔥 BURN" + RESET;
        case 3: return YELLOW + "⚡ STUN" + RESET;
        default: return GREEN + "✨ HEALTHY" + RESET;
    }
}

void TerminalHUD::printHeaderBanner(const string& title) {
    cout << CYAN << "╔══════════════════════════════════════════════════════════════════════════════╗" << RESET << endl;
    cout << CYAN << "║ " << BOLD << WHITE;
    cout.width(76);
    cout << left << title;
    cout << CYAN << " ║" << RESET << endl;
    cout << CYAN << "╚══════════════════════════════════════════════════════════════════════════════╝" << RESET << endl;
}

void TerminalHUD::flashMessage(const string& msg, const string& color) {
    cout << "\n" << color << ">>> " << BOLD << msg << RESET << "\n" << endl;
}
