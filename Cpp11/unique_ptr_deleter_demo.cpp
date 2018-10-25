std::unique_ptr<std::FILE, decltype(&std::fclose)> 
            fp(std::fopen("demo.txt", "r"), &std::fclose);

std::unique_ptr<TF_Buffer, decltype(&TF_DeleteBuffer)>
        op_list_buf(TF_GetAllOpList(), &TF_DeleteBuffer);

std::unique_ptr<TF_Status, decltype(&TF_DeleteStatus)>
        status(TF_NewStatus(), &TF_DeleteStatus);

std::unique_ptr<D, std::function<void(D*)>> p(new D, [](D* ptr)
        {
            std::cout << "destroying from a custom deleter...\n";
            delete ptr;
        });  // p owns D
    p->bar();
