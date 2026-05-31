#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include <map>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>

#pragma comment(lib, "ws2_32.lib")

// ============================================================================
// SYSTEM CONFIGURATION  (change these before deployment!)
// ============================================================================
extern const std::string ADMIN_USERNAME      = "abrar";
extern const std::string ADMIN_PASSWORD      = "abrar@admin";
extern const std::string ADMIN_COOKIE_NAME   = "admin_session";
extern const std::string ADMIN_SESSION_TOKEN = "exam_admin_secret_2026";
extern const int EXAM_DURATION_SECONDS_CFG   = 1800; // 30 minutes (used by exam timer)

#include "database.h"

// Helper split utility
std::vector<std::string> splitString(const std::string& s, char delim) {
    return dbSplitString(s, delim);
}

std::string urlDecode(const std::string& src) {
    std::string dst;
    for (size_t i = 0; i < src.length(); ++i) {
        if (src[i] == '+') {
            dst += ' ';
        } else if (src[i] == '%' && i + 2 < src.length()) {
            int value;
            std::istringstream is(src.substr(i + 1, 2));
            if (is >> std::hex >> value) {
                dst += static_cast<char>(value);
                i += 2;
            } else {
                dst += src[i];
            }
        } else {
            dst += src[i];
        }
    }
    return dst;
}

std::map<std::string, std::string> parseFormData(const std::string& body) {
    std::map<std::string, std::string> params;
    std::stringstream ss(body);
    std::string pair;
    while (std::getline(ss, pair, '&')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = urlDecode(pair.substr(0, pos));
            std::string val = urlDecode(pair.substr(pos + 1));
            params[key] = val;
        }
    }
    return params;
}


// ============================================================================
// PREMIUM FRONTEND HTML/CSS TEMPLATES (MOVED TO templates.cpp/templates.h)
// ============================================================================
#include "templates.h"

// Helper function to read a template from disk or fallback


// ============================================================================
// WEB ADMIN PANEL — AUTHENTICATION & PAGE BUILDERS
// ============================================================================










// ============================================================================
// DYNAMIC WEB ENGINE ROUTING & LOGIC
// ============================================================================
struct HTTPRequest {
    std::string method;
    std::string path;
    std::map<std::string, std::string> headers;
    std::string body;
};

HTTPRequest parseRequest(const std::string& raw) {
    HTTPRequest req;
    size_t boundary = raw.find("\r\n\r\n");
    std::string headerPart;
    if (boundary != std::string::npos) {
        headerPart = raw.substr(0, boundary);
        req.body = raw.substr(boundary + 4);
    } else {
        boundary = raw.find("\n\n");
        if (boundary != std::string::npos) {
            headerPart = raw.substr(0, boundary);
            req.body = raw.substr(boundary + 2);
        } else {
            headerPart = raw;
        }
    }
    
    std::stringstream ss(headerPart);
    std::string line;
    
    if (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::stringstream reqLine(line);
        reqLine >> req.method >> req.path;
    }
    
    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string val = line.substr(colon + 1);
            auto trim = [](std::string& str) {
                str.erase(0, str.find_first_not_of(" \t"));
                str.erase(str.find_last_not_of(" \t") + 1);
            };
            trim(key);
            trim(val);
            req.headers[key] = val;
        }
    }
    
    return req;
}

std::string getCookieValue(const std::map<std::string, std::string>& headers, const std::string& cookieName) {
    auto it = headers.find("cookie");
    if (it == headers.end()) {
        it = headers.find("Cookie");
        if (it == headers.end()) return "";
    }
    std::string cookieHeader = it->second;
    size_t pos = cookieHeader.find(cookieName + "=");
    if (pos == std::string::npos) return "";
    pos += cookieName.length() + 1;
    size_t endPos = cookieHeader.find(';', pos);
    if (endPos == std::string::npos) {
        return cookieHeader.substr(pos);
    }
    return cookieHeader.substr(pos, endPos - pos);
}

void sendHTTPResponse(SOCKET clientSocket, const std::string& status, const std::string& contentType, const std::string& body) {
    std::string response = "HTTP/1.1 " + status + "\r\n";
    response += "Content-Type: " + contentType + "; charset=utf-8\r\n";
    response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
    response += "Connection: close\r\n\r\n";
    response += body;
    send(clientSocket, response.c_str(), static_cast<int>(response.length()), 0);
}

void sendRedirect(SOCKET clientSocket, const std::string& location) {
    std::string response = "HTTP/1.1 302 Found\r\n";
    response += "Location: " + location + "\r\n";
    response += "Connection: close\r\n\r\n";
    send(clientSocket, response.c_str(), static_cast<int>(response.length()), 0);
}

void sendRedirectWithCookie(SOCKET clientSocket, const std::string& location, const std::string& cookieHeaderValue) {
    std::string response = "HTTP/1.1 302 Found\r\n";
    response += "Location: " + location + "\r\n";
    response += "Set-Cookie: " + cookieHeaderValue + "\r\n";
    response += "Connection: close\r\n\r\n";
    send(clientSocket, response.c_str(), static_cast<int>(response.length()), 0);
}

std::string getExamHTML(const std::string& roll, const std::string& name, DBManager& db) {
    std::string html = loadTemplate("exam.html", EXAM_HTML_TEMPLATE);
    
    replaceAll(html, "<!-- STUDENT_NAME -->", name);
    replaceAll(html, "<!-- STUDENT_ROLL -->", roll);
    
    std::vector<Question> questions = db.getQuestionsCopy();
    
    replaceAll(html, "<!-- TOTAL_QUESTIONS_COUNT -->", std::to_string(questions.size()));
    
    replaceAll(html, "<!-- EXAM_DURATION_SECONDS -->", std::to_string(EXAM_DURATION_SECONDS_CFG));

    std::string navButtons;
    for (size_t i = 0; i < questions.size(); ++i) {
        int idx = static_cast<int>(i + 1);
        navButtons += "<button type=\"button\" class=\"q-nav-btn\" id=\"nav_btn_" + std::to_string(idx) + 
                      "\" onclick=\"scrollToQuestion(" + std::to_string(idx) + ")\">" + std::to_string(idx) + "</button>\n";
    }
    replaceAll(html, "<!-- NAV_BUTTONS -->", navButtons);

    std::string questionCards;
    for (size_t i = 0; i < questions.size(); ++i) {
        const auto& q = questions[i];
        int idx = static_cast<int>(i + 1);
        std::string qCard = R"html(
        <div class="question-card" id="question_card_IDX">
            <div class="question-header">
                <span class="question-number">Question IDX</span>
                <span class="question-marks">MARKS Marks</span>
            </div>
            <div class="question-text">QTEXT</div>
            <div class="options-list">
                <label class="option-label">
                    <input type="radio" name="q_IDX" value="A">
                    <span>A) OPTA</span>
                </label>
                <label class="option-label">
                    <input type="radio" name="q_IDX" value="B">
                    <span>B) OPTB</span>
                </label>
                <label class="option-label">
                    <input type="radio" name="q_IDX" value="C">
                    <span>C) OPTC</span>
                </label>
                <label class="option-label">
                    <input type="radio" name="q_IDX" value="D">
                    <span>D) OPTD</span>
                </label>
            </div>
        </div>
        )html";
        
        replaceAll(qCard, "IDX", std::to_string(idx));
        replaceAll(qCard, "MARKS", std::to_string(q.marks));
        replaceAll(qCard, "QTEXT", escapeHTML(q.text));
        replaceAll(qCard, "OPTA", escapeHTML(q.optionA));
        replaceAll(qCard, "OPTB", escapeHTML(q.optionB));
        replaceAll(qCard, "OPTC", escapeHTML(q.optionC));
        replaceAll(qCard, "OPTD", escapeHTML(q.optionD));
        
        questionCards += qCard;
    }
    
    replaceAll(html, "<!-- EXAM_QUESTIONS_PLACEHOLDER -->", questionCards);
    return html;
}

std::string processExamSubmission(const std::string& roll, const std::string& name, const std::map<std::string, std::string>& params, DBManager& db) {
    std::vector<Question> questions = db.getQuestionsCopy();
    int score = 0;
    int totalMarks = 0;
    int correctCount = 0;
    int incorrectCount = 0;
    
    std::string reviewItemsHtml;
    
    for (size_t i = 0; i < questions.size(); ++i) {
        const auto& q = questions[i];
        int idx = static_cast<int>(i + 1);
        totalMarks += q.marks;
        
        std::string paramName = "q_" + std::to_string(idx);
        std::string userAnswerStr = "";
        char userAnswerChar = '\0';
        
        auto it = params.find(paramName);
        if (it != params.end() && !it->second.empty()) {
            userAnswerStr = it->second;
            userAnswerChar = userAnswerStr[0];
        }
        
        bool isCorrect = (userAnswerChar == q.correctOption);
        if (isCorrect) {
            score += q.marks;
            correctCount++;
        } else {
            incorrectCount++;
        }
        
        std::string reviewCard;
        if (isCorrect) {
            reviewCard = R"html(
            <div class="review-card correct">
                <div class="review-question">Question IDX: QTEXT</div>
                <div class="review-answers">
                    <div class="answer-badge user-answer correct-answer">
                        <span class="badge-label">Your Answer:</span> OPT_VAL) OPT_TXT
                    </div>
                </div>
            </div>
            )html";
            
            std::string optTxt = "";
            if (q.correctOption == 'A') optTxt = q.optionA;
            else if (q.correctOption == 'B') optTxt = q.optionB;
            else if (q.correctOption == 'C') optTxt = q.optionC;
            else if (q.correctOption == 'D') optTxt = q.optionD;
            
            replaceAll(reviewCard, "IDX", std::to_string(idx));
            replaceAll(reviewCard, "QTEXT", escapeHTML(q.text));
            replaceAll(reviewCard, "OPT_VAL", std::string(1, q.correctOption));
            replaceAll(reviewCard, "OPT_TXT", escapeHTML(optTxt));
        } else {
            reviewCard = R"html(
            <div class="review-card incorrect">
                <div class="review-question">Question IDX: QTEXT</div>
                <div class="review-answers">
                    <div class="answer-badge user-answer">
                        <span class="badge-label">Your Answer:</span> USER_OPT_VAL) USER_OPT_TXT
                    </div>
                    <div class="answer-badge correct-sol">
                        <span class="badge-label">Correct Solution:</span> CORRECT_OPT_VAL) CORRECT_OPT_TXT
                    </div>
                </div>
            </div>
            )html";
            
            std::string userOptTxt = "No Answer";
            if (userAnswerChar == 'A') userOptTxt = q.optionA;
            else if (userAnswerChar == 'B') userOptTxt = q.optionB;
            else if (userAnswerChar == 'C') userOptTxt = q.optionC;
            else if (userAnswerChar == 'D') userOptTxt = q.optionD;
            
            std::string corrOptTxt = "";
            if (q.correctOption == 'A') corrOptTxt = q.optionA;
            else if (q.correctOption == 'B') corrOptTxt = q.optionB;
            else if (q.correctOption == 'C') corrOptTxt = q.optionC;
            else if (q.correctOption == 'D') corrOptTxt = q.optionD;
            
            replaceAll(reviewCard, "IDX", std::to_string(idx));
            replaceAll(reviewCard, "QTEXT", escapeHTML(q.text));
            replaceAll(reviewCard, "USER_OPT_VAL", userAnswerChar != '\0' ? std::string(1, userAnswerChar) : "");
            replaceAll(reviewCard, "USER_OPT_TXT", userAnswerChar != '\0' ? escapeHTML(userOptTxt) : "Unanswered");
            replaceAll(reviewCard, "CORRECT_OPT_VAL", std::string(1, q.correctOption));
            replaceAll(reviewCard, "CORRECT_OPT_TXT", escapeHTML(corrOptTxt));
        }
        
        reviewItemsHtml += reviewCard;
    }
    
    // Timestamp
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    db.addResult(roll, score, totalMarks, oss.str());
    
    std::string html = loadTemplate("result.html", RESULT_HTML_TEMPLATE);
    
    int percent = (totalMarks > 0) ? (score * 100 / totalMarks) : 0;
    bool isPass = (percent >= 40);
    
    replaceAll(html, "<!-- STUDENT_ROLL -->", roll);
    replaceAll(html, "<!-- STUDENT_NAME -->", name);
    replaceAll(html, "<!-- PERCENT_CLASS -->", isPass ? "circle-pass" : "circle-fail");
    replaceAll(html, "<!-- PERCENT_VALUE -->", std::to_string(percent));
    replaceAll(html, "<!-- PERCENT_TEXT -->", std::to_string(percent));
    replaceAll(html, "<!-- PASS_CLASS -->", isPass ? "pass" : "fail");
    replaceAll(html, "<!-- PASS_TEXT -->", isPass ? "Passed Successfully" : "Failed / Needs Improvement");
    replaceAll(html, "<!-- SCORE_OBTAINED -->", std::to_string(score));
    replaceAll(html, "<!-- TOTAL_MARKS -->", std::to_string(totalMarks));
    replaceAll(html, "<!-- CORRECT_COUNT -->", std::to_string(correctCount));
    replaceAll(html, "<!-- INCORRECT_COUNT -->", std::to_string(incorrectCount));
    replaceAll(html, "<!-- PERFORMANCE_REVIEW_ITEMS -->", reviewItemsHtml);
    
    return html;
}

void handleClient(SOCKET clientSocket, DBManager& db) {
    std::string rawRequest;
    char buffer[4096];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        closesocket(clientSocket);
        return;
    }
    buffer[bytesReceived] = '\0';
    rawRequest.append(buffer, bytesReceived);
    
    HTTPRequest req = parseRequest(rawRequest);
    
    // Check for Content-Length to complete reading POST bodies
    auto clIt = req.headers.find("Content-Length");
    if (clIt == req.headers.end()) clIt = req.headers.find("content-length");
    
    if (clIt != req.headers.end()) {
        size_t contentLength = std::stoul(clIt->second);
        size_t boundary = rawRequest.find("\r\n\r\n");
        size_t headerSize = (boundary != std::string::npos) ? (boundary + 4) : rawRequest.find("\n\n");
        if (headerSize != std::string::npos) {
            headerSize = (boundary != std::string::npos) ? (boundary + 4) : (headerSize + 2);
        } else {
            headerSize = rawRequest.length();
        }
        
        size_t bodyBytesRead = rawRequest.length() - headerSize;
        while (bodyBytesRead < contentLength) {
            size_t toRead = std::min(sizeof(buffer) - 1, contentLength - bodyBytesRead);
            bytesReceived = recv(clientSocket, buffer, static_cast<int>(toRead), 0);
            if (bytesReceived <= 0) break;
            buffer[bytesReceived] = '\0';
            rawRequest.append(buffer, bytesReceived);
            bodyBytesRead += bytesReceived;
        }
        req = parseRequest(rawRequest);
    }
    
    std::string path = req.path;
    size_t qPos = path.find('?');
    std::string route = (qPos == std::string::npos) ? path : path.substr(0, qPos);

    if (route == "/") {
        std::string roll = getCookieValue(req.headers, "session_id");
        std::string name;
        if (!roll.empty() && db.getStudentName(roll, name)) {
            sendRedirect(clientSocket, "/exam");
        } else {
            sendHTTPResponse(clientSocket, "200 OK", "text/html", loadTemplate("login.html", LOGIN_HTML_TEMPLATE));
        }
    } 
    else if (route == "/login") {
        if (req.method == "POST") {
            auto params = parseFormData(req.body);
            std::string roll = params["roll_number"];
            std::string pass = params["password"];
            std::string sName;
            if (!roll.empty() && db.validateStudent(roll, pass, sName)) {
                sendRedirectWithCookie(clientSocket, "/exam", "session_id=" + roll + "; Path=/; HttpOnly");
            } else {
                sendRedirect(clientSocket, "/?error=1");
            }
        } else {
            sendRedirect(clientSocket, "/");
        }
    } 
    else if (route == "/exam") {
        std::string roll = getCookieValue(req.headers, "session_id");
        std::string sName;
        if (!roll.empty() && db.getStudentName(roll, sName)) {
            Result res;
            if (db.getStudentResult(roll, res)) {
                std::string html = loadTemplate("already_submitted.html", ALREADY_SUBMITTED_HTML_TEMPLATE);
                int percent = (res.totalMarks > 0) ? (res.score * 100 / res.totalMarks) : 0;
                bool isPass = (percent >= 40);
                
                replaceAll(html, "<!-- STUDENT_NAME -->", res.studentName);
                replaceAll(html, "<!-- STUDENT_ROLL -->", res.rollNumber);
                replaceAll(html, "<!-- EXAM_TIME -->", res.timestamp);
                replaceAll(html, "<!-- SCORE_OBTAINED -->", std::to_string(res.score));
                replaceAll(html, "<!-- TOTAL_MARKS -->", std::to_string(res.totalMarks));
                replaceAll(html, "<!-- PASS_CLASS -->", isPass ? "pass" : "fail");
                replaceAll(html, "<!-- PASS_TEXT -->", isPass ? "Passed" : "Failed");
                
                sendHTTPResponse(clientSocket, "200 OK", "text/html", html);
            } else {
                sendHTTPResponse(clientSocket, "200 OK", "text/html", getExamHTML(roll, sName, db));
            }
        } else {
            sendRedirect(clientSocket, "/");
        }
    } 
    else if (route == "/submit") {
        std::string roll = getCookieValue(req.headers, "session_id");
        std::string sName;
        if (!roll.empty() && db.getStudentName(roll, sName)) {
            if (req.method == "POST") {
                auto params = parseFormData(req.body);
                std::string html = processExamSubmission(roll, sName, params, db);
                sendHTTPResponse(clientSocket, "200 OK", "text/html", html);
            } else {
                sendRedirect(clientSocket, "/exam");
            }
        } else {
            sendRedirect(clientSocket, "/");
        }
    } 
    else if (route == "/logout") {
        sendRedirectWithCookie(clientSocket, "/", "session_id=; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 GMT; Max-Age=0");
    }
    // =====================================================================
    // ADMIN PANEL ROUTES
    // =====================================================================
    else if (route == "/admin" || route == "/admin/") {
        if (isAdminAuthenticated(req.headers))
            sendRedirect(clientSocket, "/admin/dashboard");
        else
            sendRedirect(clientSocket, "/admin/login");
    }
    else if (route == "/admin/login") {
        if (req.method == "POST") {
            auto params = parseFormData(req.body);
            if (params["username"] == ADMIN_USERNAME && params["password"] == ADMIN_PASSWORD) {
                sendRedirectWithCookie(clientSocket, "/admin/dashboard",
                    ADMIN_COOKIE_NAME + "=" + ADMIN_SESSION_TOKEN + "; Path=/; HttpOnly");
            } else {
                sendRedirect(clientSocket, "/admin/login?error=1");
            }
        } else {
            sendHTTPResponse(clientSocket, "200 OK", "text/html", ADMIN_LOGIN_HTML);
        }
    }
    else if (route == "/admin/logout") {
        sendRedirectWithCookie(clientSocket, "/admin/login",
            ADMIN_COOKIE_NAME + "=; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 GMT; Max-Age=0");
    }
    else if (route == "/admin/dashboard") {
        if (!isAdminAuthenticated(req.headers)) { sendRedirect(clientSocket, "/admin/login"); }
        else sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminDashboardHTML(db));
    }
    else if (route == "/admin/students") {
        if (!isAdminAuthenticated(req.headers)) { sendRedirect(clientSocket, "/admin/login"); }
        else sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminStudentsHTML(db));
    }
    else if (route == "/admin/students/add") {
        if (!isAdminAuthenticated(req.headers)) { sendRedirect(clientSocket, "/admin/login"); return; }
        auto params = parseFormData(req.body);
        std::string roll = params["roll"], name = params["name"], pass = params["password"];
        if (roll.empty() || name.empty() || pass.empty())
            sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminStudentsHTML(db, "Error: All fields are required.", true));
        else if (!db.addStudent(roll, name, pass))
            sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminStudentsHTML(db, "Error: Roll Number '" + roll + "' already exists.", true));
        else
            sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminStudentsHTML(db, "Student '" + name + "' (Roll: " + roll + ") added successfully!"));
    }
    else if (route == "/admin/students/delete") {
        if (!isAdminAuthenticated(req.headers)) { sendRedirect(clientSocket, "/admin/login"); return; }
        auto params = parseFormData(req.body);
        std::string roll = params["roll"];
        if (db.deleteStudent(roll))
            sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminStudentsHTML(db, "Student (Roll: " + roll + ") deleted successfully."));
        else
            sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminStudentsHTML(db, "Error: Roll Number not found.", true));
    }
    else if (route == "/admin/questions") {
        if (!isAdminAuthenticated(req.headers)) { sendRedirect(clientSocket, "/admin/login"); }
        else sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminQuestionsHTML(db));
    }
    else if (route == "/admin/questions/add") {
        if (!isAdminAuthenticated(req.headers)) { sendRedirect(clientSocket, "/admin/login"); return; }
        auto params = parseFormData(req.body);
        std::string text=params["text"], a=params["opta"], b=params["optb"],
                    c=params["optc"], d=params["optd"];
        std::string correctStr = params["correct"];
        int marks = 0;
        try { marks = std::stoi(params["marks"]); } catch (...) { marks = 0; }
        char correct = correctStr.empty() ? 'A' : std::toupper(correctStr[0]);
        if (text.empty() || a.empty() || b.empty() || c.empty() || d.empty() || marks <= 0)
            sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminQuestionsHTML(db, "Error: All fields are required and marks must be > 0.", true));
        else {
            db.addQuestion(text, a, b, c, d, correct, marks);
            sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminQuestionsHTML(db, "Question added successfully!"));
        }
    }
    else if (route == "/admin/questions/delete") {
        if (!isAdminAuthenticated(req.headers)) { sendRedirect(clientSocket, "/admin/login"); return; }
        auto params = parseFormData(req.body);
        int id = 0;
        try { id = std::stoi(params["id"]); } catch (...) { id = 0; }
        if (db.deleteQuestion(id))
            sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminQuestionsHTML(db, "Question deleted successfully."));
        else
            sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminQuestionsHTML(db, "Error: Question ID not found.", true));
    }
    else if (route == "/admin/results") {
        if (!isAdminAuthenticated(req.headers)) { sendRedirect(clientSocket, "/admin/login"); }
        else sendHTTPResponse(clientSocket, "200 OK", "text/html", getAdminResultsHTML(db));
    }
    else {
        sendHTTPResponse(clientSocket, "404 Not Found", "text/plain", "404 Page Not Found");
    }
    
    closesocket(clientSocket);
}

void printLocalIPs(int port) {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        std::cout << "[Server] Listening on port " << port << " (could not get local IP details)\n";
        return;
    }
    
    struct addrinfo hints, *res = NULL, *p = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
        std::cout << "[Server] Listening on port " << port << " (could not resolve IP addresses)\n";
        return;
    }

    std::cout << "\n==================================================\n";
    std::cout << "🌐 EXAM PORTAL ACCESS LINKS:\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Local PC: http://localhost:" << port << "\n";
    
    bool foundIP = false;
    for (p = res; p != NULL; p = p->ai_next) {
        struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
        char ipstr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
        
        if (std::string(ipstr) != "127.0.0.1" && std::string(ipstr) != "0.0.0.0") {
            std::cout << "Same Wi-Fi (Phone/Tablet): http://" << ipstr << ":" << port << "\n";
            foundIP = true;
        }
    }
    if (!foundIP) {
        std::cout << "(Ensure your computer is connected to Wi-Fi to access from phone)\n";
    }
    std::cout << "==================================================\n\n";

    if (res) {
        freeaddrinfo(res);
    }
}

// ============================================================================
// WEB SERVER CONTROLLER CLASS (Thread-Safe Background Execution)
// ============================================================================
class WebServer {
private:
    DBManager& db;
    SOCKET serverSocket;
    std::thread serverThread;
    std::atomic<bool> isRunning;
    int port;

    static void threadWorker(WebServer* server) {
        server->run();
    }

    void run() {
        WSADATA wsaData;
        int wsResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsResult != 0) {
            std::cerr << "[Server] WSAStartup failed: " << wsResult << std::endl;
            isRunning = false;
            return;
        }

        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "[Server] Socket creation failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            isRunning = false;
            return;
        }

        char optVal = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));

        sockaddr_in serverService;
        serverService.sin_family = AF_INET;
        serverService.sin_addr.s_addr = INADDR_ANY;
        serverService.sin_port = htons(port);

        if (bind(serverSocket, (SOCKADDR*)&serverService, sizeof(serverService)) == SOCKET_ERROR) {
            std::cerr << "[Server] Bind failed with error: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            serverSocket = INVALID_SOCKET;
            isRunning = false;
            return;
        }

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "[Server] Listen failed with error: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            serverSocket = INVALID_SOCKET;
            isRunning = false;
            return;
        }

        printLocalIPs(port);

        while (isRunning) {
            sockaddr_in clientAddr;
            int clientAddrSize = sizeof(clientAddr);
            SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
            
            if (clientSocket == INVALID_SOCKET) {
                if (!isRunning) break;
                std::cerr << "[Server] Accept failed with error: " << WSAGetLastError() << std::endl;
                continue;
            }

            std::thread clientThread([this, clientSocket]() {
                handleClient(clientSocket, db);
            });
            clientThread.detach();
        }

        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
            serverSocket = INVALID_SOCKET;
        }
        WSACleanup();
    }

public:
    WebServer(DBManager& dbRef, int portVal = 8080) 
        : db(dbRef), serverSocket(INVALID_SOCKET), isRunning(false), port(portVal) {}

    ~WebServer() {
        stop();
    }

    bool start() {
        if (isRunning) return true;
        isRunning = true;
        serverThread = std::thread(threadWorker, this);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return isRunning;
    }

    void stop() {
        if (!isRunning) return;
        isRunning = false;
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
        }
        if (serverThread.joinable()) {
            serverThread.join();
        }
        serverSocket = INVALID_SOCKET;
    }

    bool getIsRunning() const {
        return isRunning;
    }

    int getPort() const {
        return port;
    }
};

// ============================================================================
// CONSOLE ADMINISTRATION PANEL
// ============================================================================
void manageStudents(DBManager& db) {
    while (true) {
        std::cout << "\n--- Student Management ---\n";
        std::cout << "1. Add Student\n";
        std::cout << "2. View All Students\n";
        std::cout << "3. Delete Student\n";
        std::cout << "4. Back to Main Menu\n";
        std::cout << "Enter Choice: ";
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        std::cin.ignore(10000, '\n');
        
        if (choice == 4) break;
        
        switch (choice) {
            case 1: {
                std::string roll, name, pass;
                std::cout << "Enter Roll Number / Student ID: ";
                std::getline(std::cin, roll);
                std::cout << "Enter Student Name: ";
                std::getline(std::cin, name);
                std::cout << "Enter Password: ";
                std::getline(std::cin, pass);
                
                if (roll.empty() || name.empty() || pass.empty()) {
                    std::cout << "Error: Fields cannot be empty!\n";
                } else {
                    if (db.addStudent(roll, name, pass)) {
                        std::cout << "Student added successfully!\n";
                    } else {
                        std::cout << "Error: Roll Number already exists!\n";
                    }
                }
                break;
            }
            case 2: {
                auto students = db.getStudentsCopy();
                if (students.empty()) {
                    std::cout << "No students found in the database.\n";
                } else {
                    std::cout << "\n------------------------------------------------------------\n";
                    std::cout << std::left << std::setw(15) << "Roll Number" << std::setw(25) << "Name" << "Password\n";
                    std::cout << "------------------------------------------------------------\n";
                    for (const auto& s : students) {
                        std::cout << std::left << std::setw(15) << s.rollNumber << std::setw(25) << s.name << "••••••••" << "\n";
                    }
                    std::cout << "------------------------------------------------------------\n";
                }
                break;
            }
            case 3: {
                std::string roll;
                std::cout << "Enter Roll Number of the student to delete: ";
                std::getline(std::cin, roll);
                if (db.deleteStudent(roll)) {
                    std::cout << "Student deleted successfully (and results cleared)!\n";
                } else {
                    std::cout << "Student not found!\n";
                }
                break;
            }
            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    }
}

void manageQuestions(DBManager& db) {
    while (true) {
        std::cout << "\n--- Question Paper Management ---\n";
        std::cout << "1. Add Question to Paper\n";
        std::cout << "2. View Question Paper\n";
        std::cout << "3. Delete Question\n";
        std::cout << "4. Back to Main Menu\n";
        std::cout << "Enter Choice: ";
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        std::cin.ignore(10000, '\n');
        
        if (choice == 4) break;
        
        switch (choice) {
            case 1: {
                std::string text, a, b, c, d;
                char correct;
                int marks;
                
                std::cout << "Enter Question Text: ";
                std::getline(std::cin, text);
                std::cout << "Enter Option A: ";
                std::getline(std::cin, a);
                std::cout << "Enter Option B: ";
                std::getline(std::cin, b);
                std::cout << "Enter Option C: ";
                std::getline(std::cin, c);
                std::cout << "Enter Option D: ";
                std::getline(std::cin, d);
                
                std::cout << "Enter Correct Option (A/B/C/D): ";
                std::cin >> correct;
                correct = std::toupper(correct);
                
                std::cout << "Enter Marks for this question: ";
                std::cin >> marks;
                std::cin.ignore(10000, '\n');
                
                if (text.empty() || a.empty() || b.empty() || c.empty() || d.empty() || 
                    (correct != 'A' && correct != 'B' && correct != 'C' && correct != 'D') || marks <= 0) {
                    std::cout << "Error: Invalid inputs or empty fields!\n";
                } else {
                    db.addQuestion(text, a, b, c, d, correct, marks);
                    std::cout << "Question added successfully!\n";
                }
                break;
            }
            case 2: {
                auto questions = db.getQuestionsCopy();
                if (questions.empty()) {
                    std::cout << "No questions in the question paper yet.\n";
                } else {
                    for (const auto& q : questions) {
                        std::cout << "\nQ" << q.id << " (" << q.marks << " Marks): " << q.text << "\n";
                        std::cout << "  A) " << q.optionA << "\n";
                        std::cout << "  B) " << q.optionB << "\n";
                        std::cout << "  C) " << q.optionC << "\n";
                        std::cout << "  D) " << q.optionD << "\n";
                        std::cout << "  Correct Option: " << q.correctOption << "\n";
                    }
                }
                break;
            }
            case 3: {
                int id;
                std::cout << "Enter Question ID to delete: ";
                std::cin >> id;
                std::cin.ignore(10000, '\n');
                
                if (db.deleteQuestion(id)) {
                    std::cout << "Question deleted successfully!\n";
                } else {
                    std::cout << "Question ID not found!\n";
                }
                break;
            }
            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    }
}

void viewResults(DBManager& db) {
    auto results = db.getResultsCopy();
    if (results.empty()) {
        std::cout << "\nNo exam submissions recorded yet.\n";
    } else {
        std::cout << "\n--------------------------------------------------------------------------------------\n";
        std::cout << std::left << std::setw(15) << "Roll Number" << std::setw(25) << "Student Name" 
                  << std::setw(12) << "Score" << std::setw(10) << "Percent" << "Submission Time\n";
        std::cout << "--------------------------------------------------------------------------------------\n";
        for (const auto& r : results) {
            int pct = r.totalMarks > 0 ? (r.score * 100 / r.totalMarks) : 0;
            std::string scoreStr = std::to_string(r.score) + "/" + std::to_string(r.totalMarks);
            std::string pctStr = std::to_string(pct) + "%";
            std::cout << std::left << std::setw(15) << r.rollNumber << std::setw(25) << r.studentName 
                      << std::setw(12) << scoreStr << std::setw(10) << pctStr << r.timestamp << "\n";
        }
        std::cout << "--------------------------------------------------------------------------------------\n";
    }
}

void runAdminPanel(DBManager& db, WebServer& server) {
    while (true) {
        std::cout << "\n============================================================\n";
        std::cout << "          ONLINE EXAMINATION SYSTEM  —  Admin Console       \n";
        std::cout << "============================================================\n";
        std::cout << "  Server : " << (server.getIsRunning()
            ? "RUNNING  →  http://localhost:" + std::to_string(server.getPort()) + "  |  /admin for Web Panel"
            : "STOPPED") << "\n";
        std::cout << "------------------------------------------------------------\n";
        std::cout << "  SERVER\n";
        std::cout << "    1. Start Web Server\n";
        std::cout << "    2. Stop  Web Server\n";
        std::cout << "  DATA MANAGEMENT\n";
        std::cout << "    3. Manage Students      (Add / List / Delete)\n";
        std::cout << "    4. Manage Questions     (Add / List / Delete)\n";
        std::cout << "    5. View Exam Results\n";
        std::cout << "  UTILITIES\n";
        std::cout << "    6. Load Sample Data     (overwrites existing students & questions)\n";
        std::cout << "    7. Clear ALL Data       (wipes students, questions AND results)\n";
        std::cout << "    8. Exit\n";
        std::cout << "------------------------------------------------------------\n";
        std::cout << "  Enter Choice (1-8): ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "  [!] Invalid input — please enter a number between 1 and 8.\n";
            std::cin.get();
            continue;
        }
        std::cin.ignore(10000, '\n');

        switch (choice) {
            case 1: {
                if (server.getIsRunning()) {
                    std::cout << "  [!] Server is already running!\n";
                } else {
                    std::cout << "  Starting web server...\n";
                    if (server.start()) {
                        std::cout << "  [OK] Server started on port " << server.getPort() << "\n";
                        std::cout << "       Students  ->  http://localhost:" << server.getPort() << "\n";
                        std::cout << "       Admin Web ->  http://localhost:" << server.getPort() << "/admin\n";
                        std::cout << "       Admin login: " << ADMIN_USERNAME << " / ••••••••\n";
                    } else {
                        std::cout << "  [ERR] Failed to start server. Is port " << server.getPort() << " already in use?\n";
                    }
                }
                break;
            }
            case 2: {
                if (!server.getIsRunning()) {
                    std::cout << "  [!] Server is not running.\n";
                } else {
                    server.stop();
                    std::cout << "  [OK] Server stopped.\n";
                }
                break;
            }
            case 3:
                manageStudents(db);
                break;
            case 4:
                manageQuestions(db);
                break;
            case 5:
                viewResults(db);
                break;
            case 6: {
                std::cout << "  [!] This will REPLACE all existing students and questions with sample data.\n";
                std::cout << "      Results will NOT be deleted. Continue? (y/n): ";
                char confirm; std::cin >> confirm; std::cin.ignore(10000, '\n');
                if (confirm == 'y' || confirm == 'Y') {
                    db.loadSampleData();
                    std::cout << "  [OK] Sample data loaded:\n";
                    std::cout << "       Students : Roll 101 (Abraar Beigh / ••••), 102 (John Doe / ••••), 103 (Alice Smith / ••••)\n";
                    std::cout << "       Questions: 5 C++ multiple-choice questions (2 marks each = 10 total)\n";
                } else {
                    std::cout << "  [--] Cancelled.\n";
                }
                break;
            }
            case 7: {
                std::cout << "  [!] WARNING: This will permanently delete ALL students, questions and results.\n";
                std::cout << "      Type 'DELETE' to confirm: ";
                std::string confirm; std::getline(std::cin, confirm);
                if (confirm == "DELETE") {
                    // Remove data files directly
                    std::remove("data/students.txt");
                    std::remove("data/questions.txt");
                    std::remove("data/results.txt");
                    std::cout << "  [OK] All data files cleared. The system is now in a fresh state.\n";
                    std::cout << "       Restart the program or use Option 6 to load sample data.\n";
                } else {
                    std::cout << "  [--] Cancelled — data was NOT deleted.\n";
                }
                break;
            }
            case 8:
                std::cout << "  Stopping server and exiting...\n";
                server.stop();
                return;
            default:
                std::cout << "  [!] Invalid choice. Enter a number between 1 and 8.\n";
                break;
        }
        std::cout << "\n  Press Enter to return to menu...";
        std::cin.get();
    }
}

// ============================================================================
// MAIN APPLICATION ENTRY POINT
// ============================================================================
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    std::cout << "Initializing Online Examination System Database...\n";
    DBManager db;
    
    // Create templates folder and physical HTML files if they don't exist (Forces Overwrite Now)
    createTemplatesIfNeeded();
    
    // Create server listening on localhost port 8080
    WebServer server(db, 8080);
    
    std::cout << "Database loaded successfully.\n";
    std::cout << "Starting Admin Panel Console...\n";
    
    runAdminPanel(db, server);
    
    return 0;
}