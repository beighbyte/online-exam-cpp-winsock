#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <string>
#include <map>
#include "database.h"

// HTML Template Constants
extern const std::string ALREADY_SUBMITTED_HTML_TEMPLATE;
extern const std::string ADMIN_LOGIN_HTML;
extern const std::string LOGIN_HTML_TEMPLATE;
extern const std::string EXAM_HTML_TEMPLATE;
extern const std::string RESULT_HTML_TEMPLATE;

// Helper function to read a template from disk or fallback
std::string loadTemplate(const std::string& filename, const std::string& fallback);

// Reusable String & HTML Utilities
void replaceAll(std::string& str, const std::string& from, const std::string& to);
std::string escapeHTML(const std::string& data);

// Force template file generation on disk
void createTemplatesIfNeeded();

// Admin authenticated helper
bool isAdminAuthenticated(const std::map<std::string, std::string>& headers);

// Admin dashboard & HTML generation functions
std::string buildAdminLayout(const std::string& pageTitle,
                             const std::string& content,
                             const std::string& activeTab);

std::string getAdminDashboardHTML(DBManager& db);
std::string getAdminStudentsHTML(DBManager& db, const std::string& msg = "", bool isErr = false);
std::string getAdminQuestionsHTML(DBManager& db, const std::string& msg = "", bool isErr = false);
std::string getAdminResultsHTML(DBManager& db);

#endif // TEMPLATES_H
