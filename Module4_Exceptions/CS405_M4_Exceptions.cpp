// Exceptions.cpp
// Demonstrates standard, custom, specific, and catch-all exception handling.

#include <exception>
#include <iostream>
#include <stdexcept>

// Custom exception derived from std::exception.
class CustomApplicationException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "A custom application exception occurred.";
    }
};

bool do_even_more_custom_application_logic()
{
    // Throw a standard exception to simulate an application error.
    throw std::runtime_error(
        "The even more custom application logic encountered an error.");

    std::cout << "Running Even More Custom Application Logic."
        << std::endl;

    return true;
}

void do_custom_application_logic()
{
    std::cout << "Running Custom Application Logic." << std::endl;

    try
    {
        // This function may throw a standard C++ exception.
        if (do_even_more_custom_application_logic())
        {
            std::cout << "Even More Custom Application Logic Succeeded."
                << std::endl;
        }
    }
    catch (const std::exception& exception)
    {
        // Display the standard exception message and continue processing.
        std::cerr << "Standard exception caught in "
            << "do_custom_application_logic(): "
            << exception.what() << std::endl;
    }

    // Throw a custom exception that will be caught explicitly in main.
    throw CustomApplicationException();

    std::cout << "Leaving Custom Application Logic." << std::endl;
}

float divide(float numerator, float denominator)
{
    // Prevent division by zero by throwing a standard C++ exception.
    if (denominator == 0.0f)
    {
        throw std::invalid_argument(
            "Division cannot be performed because the denominator is zero.");
    }

    return numerator / denominator;
}

void do_division() noexcept
{
    float numerator = 10.0f;
    float denominator = 0.0f;

    try
    {
        const float result = divide(numerator, denominator);

        std::cout << "divide(" << numerator << ", " << denominator
            << ") = " << result << std::endl;
    }
    catch (const std::invalid_argument& exception)
    {
        // Catch only the exception type thrown by divide().
        std::cerr << "Division exception caught: "
            << exception.what() << std::endl;
    }
}

int main()
{
    std::cout << "Exceptions Tests!" << std::endl;

    try
    {
        // Run both exception-handling demonstrations.
        do_division();
        do_custom_application_logic();
    }
    catch (const CustomApplicationException& exception)
    {
        // Catch the custom exception before the general std::exception type.
        std::cerr << "Custom exception caught in main: "
            << exception.what() << std::endl;
    }
    catch (const std::exception& exception)
    {
        // Catch any remaining standard-library exception.
        std::cerr << "Standard exception caught in main: "
            << exception.what() << std::endl;
    }
    catch (...)
    {
        // Catch any exception not handled by the preceding handlers.
        std::cerr << "An unknown exception was caught in main."
            << std::endl;
    }

    std::cout << "Exception testing completed safely." << std::endl;

    return 0;
}