std::unique_ptr<std::FILE, decltype(&std::fclose)> fp(std::fopen("demo.txt", "r"),
                                                            &std::fclose);

std::unique_ptr<D, std::function<void(D*)>> p(new D, [](D* ptr)
        {
            std::cout << "destroying from a custom deleter...\n";
            delete ptr;
        });  // p owns D
    p->bar();
