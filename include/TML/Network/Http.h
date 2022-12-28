#pragma once
#include <TML/Export.h>
#include <TML/Network/TcpSocket.h>
#include <string>
#include <map>

namespace tml
{
    class TML_API HttpRequest
    {
    public:
        enum class Method : uint32_t
		{
			Get,
			Head,
			Post,
			Put,
			Delete,
			Connect,
			Options,
			Trace,
			Patch,
		};

    public:
        HttpRequest();
        HttpRequest(Method method, std::string URI, std::string body = "");

        void SetVersion(uint32_t major, uint32_t minor);
        void SetMethod(Method method);
        void SetURI(std::string URI);
        void SetProperty(std::string property, std::string value); /// Set header fields.
        void SetBody(std::string body);
        [[nodiscard]] std::string GetRequestString() const noexcept;
        friend class HttpClient;
    private:
        enum Method m_method;
        uint32_t m_versionMajor = 1;
        uint32_t m_versionMinor = 1;
        std::map<std::string, std::string> m_properties;
        std::string m_uri;
        std::string m_body;
    };

    class TML_API HttpResponse
    {
    public:
        enum class [[maybe_unused]] StatusCode : uint32_t
        {
            /// Information responses
            Continue                        = 100,
            Switching_Protocols             = 101,
            Processing                      = 102,
            Early_Hints                     = 103,

            /// Successful responses
            OK                              = 200,
            Created                         = 201,
            Accepted                        = 202,
            Non_Authoritative_Information   = 203,
            No_Content                      = 204,
            Reset_Content                   = 205,
            Partial_Content                 = 206,
            Multi_Status                    = 207,
            Already_Reported                = 208,
            IM_Used                         = 226,

            /// Redirection messages
            Multiple_Choice                 = 300,
            Moved_Permanently               = 301,
            Found                           = 302,
            See_Other                       = 303,
            Not_Modified                    = 304,
            Use_Proxy                       = 305,
            Unused                          = 306,
            Temporary_Redirect              = 307,
            Permanent_Redirect              = 308,

            /// Client error responses
            Bad_Request                     = 400,
            Unauthorized                    = 401,
            Payment_Required                = 402,
            Forbidden                       = 403,
            Not_Found                       = 404,
            Method_Not_Allowed              = 405,
            Not_Acceptable                  = 406,
            Proxy_Authentication_Failed     = 407,
            Request_Timeout                 = 408,
            Conflict                        = 409,
            Gone                            = 410,
            Length_Required                 = 411,
            Precondition_Failed             = 412,
            Payload_Too_Large               = 413,
            URI_Too_Long                    = 414,
            Unsupported_Media_Type          = 415,
            Range_Not_Satisfiable           = 416,
            Expectation_Failed              = 417,
            Im_a_teapot                     = 418,
            Misdirected_Request             = 421,
            Unprocessable_Entity            = 422,
            Locked                          = 423,
            Failed_Dependency               = 424,
            Too_Early                       = 425,
            Upgrade_Required                = 426,
            Precondition_Required           = 428,
            Too_Many_Requests               = 429,
            Request_Header_Fields_Too_Large = 431,
            Unavailable_For_Legal_Reasons   = 451,

            /// Server error responses
            Internal_Server_Error           = 500,
            Not_Implemented                 = 501,
            Bad_Gateway                     = 502,
            Service_Unavailable             = 503,
            Gateway_Timeout                 = 504,
            HTTP_Version_Not_Supported      = 505,
            Variant_Also_Negotiates         = 506,
            Insufficient_Storage            = 507,
            Loop_Detected                   = 508,
            Not_Extended                    = 510,
            Network_Authentication_Required = 511,
        };

    public:
        void SetProperty(const std::string& property, const std::string& value) noexcept;
        void SetStatusCode(StatusCode code) noexcept;
        void SetVersion(uint32_t major, uint32_t minor) noexcept;
        void SetBody(const std::string& content) noexcept;
        void SetContent(const std::string& content);

        [[nodiscard]] std::string GetProperty(const std::string& property) const noexcept;
        [[nodiscard]] std::string GetStatusText() const noexcept;
        [[nodiscard]] std::string GetStatusLine() const noexcept;
        [[nodiscard]] std::string GetBody() const noexcept;
        [[nodiscard]] StatusCode GetStatusCode() const noexcept;
        [[nodiscard]] std::string GetContent() const noexcept;

        friend class HttpClient;
    protected:
        void StatusFromString(const std::string& str);
        static std::string GetStatusTextFromStatusCode(StatusCode code) noexcept;

    protected:
        uint32_t m_versionMajor = 1;
        uint32_t m_versionMinor = 1;
        StatusCode m_statusCode;
        std::string m_body;
        std::map<std::string, std::string> m_properties;
    };

    class TML_API HttpClient
    {
    public:
        HttpClient() = default;
        HttpClient(std::string address, uint32_t port);

        bool Connect(std::string address, uint32_t port) noexcept;
        bool Send(HttpRequest& request) noexcept;
        bool GetResponse(HttpResponse& response) noexcept;
        std::string GetResponseString() noexcept;

    private:
        std::string m_address;
        TcpSocket m_socket;
    };
}