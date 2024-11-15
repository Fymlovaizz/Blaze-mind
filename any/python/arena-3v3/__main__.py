from heroes import _index_, _families_, _heroes_

_tab_ = "    "
_first_size_title_ = 4
_size_title_ = 100
_size_name_hero_ = 25

def title(text: str) -> None:
    print('\033c')
    print('-' * _first_size_title_, text.upper(), '-' * (_size_title_ - 2 - _first_size_title_ - len(text)), end="\n\n")
    return None
    
def option(question: str, *options: str) -> int:
    for idx, opt in enumerate(options):
        print(_tab_ + str(idx + 1), opt)

    print('\n', question.capitalize(), sep="")
    anwser = input(">>> ")

    try:
        anwser = int(anwser)
        if (anwser > 0 and anwser <= len(options)):
            return anwser
        else:
            input("The number must > 0 and <= number of options! Press [Enter] to continue...")
    except:
        if (anwser in options):
            return options.index(anwser) + 1
        else:
            input("Input must be number! Press [Enter] to continue...")
    return -1

def option_plus(question: str, end_opt: str, options: list[str]) -> int:
    for idx, opt in enumerate(options):
        if (idx != 0 and idx % 4 == 0):
            print()
        print(f"{idx + 1}. {opt}".ljust(_size_name_hero_), end="")
    else:
        print('\n0. ' + end_opt)

    print('\n', question.capitalize(), sep="")
    anwser = input(">>> ")

    try:
        anwser = int(anwser)
        if (anwser >= 0 and anwser <= len(options)):
            return anwser
        else:
            input("The number must >= 0 and <= number of options! Press [Enter] to continue...")
    except:
        if (anwser == end_opt):
            return 0
        elif (anwser in options):
            return options.index(anwser) + 1
        else:
            input("Input must be number! Press [Enter] to continue...")
    return -1

def __print_hero_library__(hero: dict) -> None:
    title(f"{hero["name"].upper()} INFORMATION")
    print("Tên", hero["name"], sep=": ")
    print("Nguyên tố", hero["families"], sep=": ")
    for idx, vle in enumerate(hero["index"]):
        print(_index_[idx], '/' * (1 + vle // 3), vle, sep=": ")
    print("Total index", sum(hero["index"]), sep=": ")
    input("Press [Enter] to continue...")

def __library_hero__() -> None:
    __running__ = True
    while __running__:
        title("hero library")
        anwser = option_plus(
            "what hero you want to check?",
            "return",
            [hero["name"] for hero in _heroes_]
        )
        match anwser:
            case 0:
                __running__ = False
            case -1:
                pass
            case _:
                __print_hero_library__(_heroes_[anwser - 1])

def __library__() -> None:
    __running__ = True
    while __running__:
        title("library")
        match option(
            "what choose you want?", 
            "exit",
            "hero",
            "skill",
            "family",
        ):
            case 1:
                __running__ = False
            case 2:
                __library_hero__()
            case 3:
                pass
            case 4:
                pass
            case _:
                pass

def __home__() -> None:
    __running__ = True
    while __running__:
        title("home")
        match option(
            "what choose you want?",
            "exit",
            "game",
            "library",
            "create",
            "setting"
        ):
            case 1:
                __running__ = False
            case 2:
                pass
            case 3:
                __library__()
            case 4:
                pass
            case 5:
                pass
            case _:
                pass

if __name__ == "__main__":
    __home__()
