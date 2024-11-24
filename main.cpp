// from isobar import *
// import logging

// def main():
//     series = PRange(60, 73, 1)
//     timeline = Timeline(120)

//     timeline.schedule({
//         "note": series,
//         "duration": 1
//     })

//     timeline.run()


// if __name__ == "__main__":
//     logging.basicConfig(level=logging.INFO, format="[%(asctime)s] %(message)s")
//     main()

#include <iostream>
#include "Pattern.h"
#include "Timeline.h"

int main()
{
    std::cout << "sheesh" << std::endl;
    auto series = PRange (60, 73, 1);
    auto timeline = std::make_unique<Timeline> (120, 480);
    return 0;
}
