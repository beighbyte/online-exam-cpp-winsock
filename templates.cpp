#include "templates.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

// ============================================================================
// PREMIUM FRONTEND HTML/CSS TEMPLATES
// ============================================================================
const std::string ALREADY_SUBMITTED_HTML_TEMPLATE = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Academy Portal - Already Submitted</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600&family=Playfair+Display:wght@600;700&display=swap" rel="stylesheet">
    <style>
        * { box-sizing: border-box; margin: 0; padding: 0; }
        body { background-color: #fdfbf7; color: #2b1e16; font-family: 'Inter', sans-serif; min-height: 100vh; display: flex; align-items: center; justify-content: center; padding: 20px; }
        .container { width: 100%; max-width: 550px; background: #ffffff; border: 1px solid #e2d9c8; border-radius: 12px; padding: 48px 40px; box-shadow: 0 10px 30px rgba(43, 30, 22, 0.05); text-align: center; }
        .icon-container { width: 80px; height: 80px; background: #f4eee4; border: 2px solid #8a9a86; border-radius: 50%; display: flex; align-items: center; justify-content: center; margin: 0 auto 24px; color: #8a9a86; font-size: 36px; font-weight: bold; }
        h2 { font-family: 'Playfair Display', serif; font-size: 28px; font-weight: 700; margin-bottom: 8px; color: #2b1e16; }
        p.subtitle { color: #6b5c51; font-size: 16px; margin-bottom: 32px; }
        .result-box { background: #fdfbf7; border: 1px solid #e2d9c8; border-radius: 12px; padding: 24px; margin-bottom: 32px; }
        .student-info { display: flex; justify-content: space-between; margin-bottom: 20px; padding-bottom: 12px; border-bottom: 1px solid #e2d9c8; font-size: 15px; }
        .student-info span.val { font-weight: 600; color: #2b1e16; }
        .student-info span.lbl { color: #8a9a86; font-weight: 500; }
        .stats-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 16px; margin-top: 16px; }
        .stat-card { background: #ffffff; border: 1px solid #d4c5b0; border-radius: 8px; padding: 16px; }
        .stat-lbl { font-size: 12px; color: #8a9a86; text-transform: uppercase; letter-spacing: 0.5px; margin-bottom: 4px; font-weight: 600; }
        .stat-val { font-size: 20px; font-weight: 700; color: #c87a53; font-family: 'Playfair Display', serif; }
        .badge { display: inline-block; padding: 6px 16px; border-radius: 20px; font-size: 13px; font-weight: 600; text-transform: uppercase; letter-spacing: 0.5px; }
        .badge.pass { background: #f4eee4; color: #758572; border: 1px solid #8a9a86; }
        .badge.fail { background: #fdf3f2; color: #c0392b; border: 1px solid #e8b0b0; }
        .btn-action { display: block; width: 100%; background: #c87a53; color: white; border: none; border-radius: 8px; padding: 14px; font-size: 16px; font-weight: 500; text-decoration: none; transition: all 0.3s ease; }
        .btn-action:hover { background: #b06846; transform: translateY(-1px); }
    </style>
</head>
<body>
    <div class="container">
        <div class="icon-container">✓</div>
        <h2>Exam Completed</h2>
        <p class="subtitle">You have already submitted your response for this examination.</p>
        <div class="result-box">
            <div class="student-info"><span class="lbl">Student Name</span><span class="val"><!-- STUDENT_NAME --></span></div>
            <div class="student-info"><span class="lbl">Roll Number</span><span class="val"><!-- STUDENT_ROLL --></span></div>
            <div class="student-info"><span class="lbl">Submitted On</span><span class="val"><!-- EXAM_TIME --></span></div>
            <div class="stats-grid">
                <div class="stat-card">
                    <div class="stat-lbl">Score</div>
                    <div class="stat-val"><!-- SCORE_OBTAINED --> / <!-- TOTAL_MARKS --></div>
                </div>
                <div class="stat-card">
                    <div class="stat-lbl">Status</div>
                    <div style="margin-top: 6px;"><span class="badge <!-- PASS_CLASS -->"><!-- PASS_TEXT --></span></div>
                </div>
            </div>
        </div>
        <a href="/logout" class="btn-action">Log Out & Exit</a>
    </div>
</body>
</html>
)html";

const std::string ADMIN_LOGIN_HTML = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1.0">
    <title>Admin Login — Academy Portal</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600&family=Playfair+Display:wght@600;700&display=swap" rel="stylesheet">
    <style>
        * { box-sizing:border-box; margin:0; padding:0; }
        body { background-color: #fdfbf7; color: #2b1e16; font-family: 'Inter', sans-serif; min-height: 100vh; display: flex; align-items: center; justify-content: center; }
        .card { width: 100%; max-width: 410px; margin: 20px; background: #ffffff; border: 1px solid #e2d9c8; border-radius: 12px; padding: 48px 40px; box-shadow: 0 10px 30px rgba(43, 30, 22, 0.05); text-align: center; }
        .badge { display: inline-block; padding: 6px 14px; border-radius: 20px; background: #f4eee4; border: 1px solid #8a9a86; color: #758572; font-size: 11px; font-weight: 600; text-transform: uppercase; letter-spacing: 1px; margin-bottom: 24px; }
        h2 { font-family: 'Playfair Display', serif; font-size: 28px; font-weight: 700; margin-bottom: 8px; color: #2b1e16; }
        .sub { color: #6b5c51; font-size: 14px; margin-bottom: 32px; }
        .grp { margin-bottom: 20px; text-align: left; }
        .grp label { display: block; font-size: 12px; font-weight: 600; color: #8a9a86; text-transform: uppercase; letter-spacing: 0.5px; margin-bottom: 8px; }
        .grp input { width: 100%; padding: 14px 16px; background: #fdfbf7; border: 1px solid #d4c5b0; border-radius: 8px; color: #2b1e16; font-size: 14px; font-family: 'Inter', sans-serif; transition: all .3s; }
        .grp input:focus { outline: none; border-color: #8a9a86; background: #ffffff; box-shadow: 0 0 0 3px rgba(138, 154, 134, 0.15); }
        .btn { width: 100%; padding: 16px; margin-top: 10px; background: #8a9a86; border: none; border-radius: 8px; color: #fff; font-size: 15px; font-weight: 500; font-family: 'Inter', sans-serif; cursor: pointer; transition: all .3s; }
        .btn:hover { background: #758572; transform: translateY(-1px); }
        .err { margin-top: 15px; padding: 12px; border-radius: 8px; background: #fdf3f2; border: 1px solid #e8b0b0; color: #c0392b; font-size: 13px; display: none; text-align: center; }
        .foot { margin-top: 32px; text-align: center; font-size: 12px; color: #a4968c; }
    </style>
</head>
<body>
    <div class="card">
        <div class="badge">🏛️ Administrator</div>
        <h2>Admin Panel</h2>
        <p class="sub">Restricted access — enter credentials</p>
        <form action="/admin/login" method="POST" id="aForm">
            <div class="grp"><label>Username</label>
                <input type="text" name="username" required placeholder="admin" autocomplete="off">
            </div>
            <div class="grp"><label>Password</label>
                <input type="password" name="password" required placeholder="••••••••">
            </div>
            <button class="btn" type="submit">Access Admin Panel →</button>
        </form>
        <div class="err" id="errDiv">Incorrect username or password.</div>
        <div class="foot">Academy Examination System • C++ Backend</div>
    </div>
    <script>if(new URLSearchParams(window.location.search).has('error'))document.getElementById('errDiv').style.display='block';</script>
</body>
</html>
)html";

const std::string LOGIN_HTML_TEMPLATE = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Academy Portal - Login</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600&family=Playfair+Display:wght@600;700&display=swap" rel="stylesheet">
    <style>
        * { box-sizing: border-box; margin: 0; padding: 0; }
        body { background-color: #fdfbf7; color: #2b1e16; font-family: 'Inter', sans-serif; min-height: 100vh; display: flex; align-items: center; justify-content: center; }
        .container { width: 100%; max-width: 450px; padding: 20px; }
        .login-card { background: #ffffff; border: 1px solid #e2d9c8; border-radius: 12px; padding: 48px 40px; box-shadow: 0 10px 30px rgba(43, 30, 22, 0.05); text-align: center; animation: fadeIn 0.6s ease-out; }
        @keyframes fadeIn { from { opacity: 0; transform: translateY(10px); } to { opacity: 1; transform: translateY(0); } }
        .logo-icon { font-size: 2.5rem; margin-bottom: 12px; color: #c87a53; }
        h2 { font-family: 'Playfair Display', serif; font-weight: 700; font-size: 2rem; margin-bottom: 8px; color: #2b1e16; }
        .subtitle { color: #6b5c51; font-size: 0.95rem; margin-bottom: 32px; }
        .input-group { margin-bottom: 20px; text-align: left; }
        .input-group label { display: block; font-size: 0.8rem; font-weight: 600; color: #8a9a86; margin-bottom: 8px; text-transform: uppercase; letter-spacing: 0.5px; }
        .input-wrapper input { width: 100%; padding: 14px 16px; background: #fdfbf7; border: 1px solid #d4c5b0; border-radius: 8px; color: #2b1e16; font-size: 1rem; font-family: 'Inter', sans-serif; transition: all 0.3s; }
        .input-wrapper input:focus { outline: none; border-color: #c87a53; background: #ffffff; box-shadow: 0 0 0 3px rgba(200, 122, 83, 0.15); }
        .btn-submit { width: 100%; padding: 16px; background: #c87a53; border: none; border-radius: 8px; color: #fff; font-size: 1rem; font-weight: 500; font-family: 'Inter', sans-serif; cursor: pointer; transition: all 0.3s; margin-top: 10px; }
        .btn-submit:hover { background: #b06846; transform: translateY(-1px); }
        .error-message { margin-top: 15px; padding: 12px; border-radius: 8px; background: #fdf3f2; border: 1px solid #e8b0b0; color: #c0392b; font-size: 0.85rem; display: none; }
        .footer-note { margin-top: 32px; font-size: 0.8rem; color: #a4968c; }
    </style>
</head>
<body>
    <div class="container">
        <div class="login-card">
            <div class="logo-icon">🏛️</div>
            <h2>Academy Portal</h2>
            <p class="subtitle">Enter your roll number and password to begin</p>
            <form action="/login" method="POST" id="loginForm">
                <div class="input-group">
                    <label for="roll_number">Roll Number / ID</label>
                    <div class="input-wrapper"><input type="text" id="roll_number" name="roll_number" required placeholder="e.g. 101" autocomplete="off"></div>
                </div>
                <div class="input-group">
                    <label for="password">Password</label>
                    <div class="input-wrapper"><input type="password" id="password" name="password" required placeholder="••••••••"></div>
                </div>
                <button type="submit" class="btn-submit">Sign In & Start</button>
            </form>
            <div class="footer-note">Secured Localhost Session &bull; C++ Backend</div>
        </div>
    </div>
    <script>
        const urlParams = new URLSearchParams(window.location.search);
        if (urlParams.has('error')) {
            const errDiv = document.createElement('div');
            errDiv.className = 'error-message'; errDiv.style.display = 'block'; errDiv.innerText = 'Invalid Roll Number or Password';
            document.getElementById('loginForm').insertBefore(errDiv, document.querySelector('.btn-submit'));
        }
    </script>
</body>
</html>
)html";

const std::string EXAM_HTML_TEMPLATE = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Academy Portal - Examination</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600&family=Playfair+Display:wght@600;700&display=swap" rel="stylesheet">
    <style>
        * { box-sizing: border-box; margin: 0; padding: 0; }
        body { background-color: #fdfbf7; color: #2b1e16; font-family: 'Inter', sans-serif; min-height: 100vh; display: flex; flex-direction: column; }
        header { background: #ffffff; border-bottom: 1px solid #e2d9c8; padding: 15px 40px; display: flex; justify-content: space-between; align-items: center; position: sticky; top: 0; z-index: 100; }
        .header-title { font-family: 'Playfair Display', serif; font-weight: 700; font-size: 1.4rem; color: #2b1e16; }
        .student-badge { background: #fdfbf7; border: 1px solid #e2d9c8; border-radius: 8px; padding: 8px 16px; display: flex; align-items: center; gap: 12px; }
        .student-info { text-align: right; }
        .student-name { font-weight: 600; font-size: 0.95rem; color: #2b1e16; }
        .student-roll { font-size: 0.8rem; color: #6b5c51; }
        .main-layout { flex: 1; display: grid; grid-template-columns: 280px 1fr; max-width: 1300px; width: 100%; margin: 40px auto; gap: 40px; padding: 0 20px; }
        .sidebar { position: sticky; top: 100px; height: fit-content; display: flex; flex-direction: column; gap: 24px; }
        .sidebar-card { background: #ffffff; border: 1px solid #e2d9c8; border-radius: 12px; padding: 24px; box-shadow: 0 4px 15px rgba(43, 30, 22, 0.03); }
        .sidebar-card h3 { font-size: 0.85rem; font-weight: 600; color: #8a9a86; margin-bottom: 15px; text-transform: uppercase; letter-spacing: 0.5px; border-bottom: 1px solid #f4eee4; padding-bottom: 10px; }
        .timer-display { font-family: 'Playfair Display', serif; font-size: 2.5rem; font-weight: 700; text-align: center; font-variant-numeric: tabular-nums; color: #c87a53; margin: 10px 0; }
        .question-grid { display: grid; grid-template-columns: repeat(5, 1fr); gap: 10px; }
        .q-nav-btn { aspect-ratio: 1; border: 1px solid #d4c5b0; border-radius: 6px; background: #fdfbf7; color: #6b5c51; font-size: 0.9rem; font-weight: 500; cursor: pointer; display: flex; align-items: center; justify-content: center; transition: all 0.2s; }
        .q-nav-btn:hover { border-color: #c87a53; color: #c87a53; }
        .q-nav-btn.answered { background: #8a9a86; border-color: #758572; color: #fff; }
        .q-nav-btn.current { background: #c87a53; border-color: #b06846; color: #fff; }
        .content-area { display: flex; flex-direction: column; gap: 24px; }
        .exam-instructions { background: #f9f7f1; border: 1px solid #d4c5b0; border-radius: 12px; padding: 20px; font-size: 0.9rem; color: #4a3e35; line-height: 1.6; }
        .question-card { background: #ffffff; border: 1px solid #e2d9c8; border-radius: 12px; padding: 35px; transition: border-color 0.3s; scroll-margin-top: 100px; box-shadow: 0 4px 15px rgba(43, 30, 22, 0.02); }
        .question-card:hover { border-color: #d4c5b0; }
        .question-header { display: flex; justify-content: space-between; align-items: flex-start; margin-bottom: 24px; }
        .question-number { font-size: 0.85rem; font-weight: 600; color: #8a9a86; text-transform: uppercase; letter-spacing: 1px; }
        .question-marks { font-size: 0.8rem; background: #f4eee4; padding: 4px 12px; border-radius: 20px; color: #6b5c51; font-weight: 500; }
        .question-text { font-family: 'Playfair Display', serif; font-size: 1.3rem; font-weight: 600; color: #2b1e16; line-height: 1.5; margin-bottom: 25px; }
        .options-list { display: flex; flex-direction: column; gap: 12px; }
        .option-label { display: flex; align-items: center; padding: 16px 20px; background: #fdfbf7; border: 1px solid #e2d9c8; border-radius: 8px; cursor: pointer; transition: all 0.2s; color: #4a3e35; }
        .option-label:hover { background: #f9f6f0; border-color: #d4c5b0; }
        .option-label input[type="radio"] { appearance: none; width: 20px; height: 20px; border: 2px solid #d4c5b0; border-radius: 50%; margin-right: 15px; display: flex; align-items: center; justify-content: center; transition: all 0.2s; cursor: pointer; }
        .option-label input[type="radio"]::before { content: ''; width: 10px; height: 10px; border-radius: 50%; background-color: #c87a53; transform: scale(0); transition: transform 0.2s; }
        .option-label input[type="radio"]:checked { border-color: #c87a53; }
        .option-label input[type="radio"]:checked::before { transform: scale(1); }
        .option-label.checked { background: #f9f7f1; border-color: #c87a53; }
        .submit-section { margin-top: 20px; margin-bottom: 80px; text-align: right; }
        .btn-submit-exam { padding: 16px 40px; background: #8a9a86; border: none; border-radius: 8px; color: #fff; font-size: 1rem; font-weight: 500; font-family: 'Inter', sans-serif; cursor: pointer; transition: all 0.3s; }
        .btn-submit-exam:hover { background: #758572; transform: translateY(-2px); }
        .modal { position: fixed; top: 0; left: 0; width: 100%; height: 100%; background: rgba(43, 30, 22, 0.6); backdrop-filter: blur(4px); display: flex; align-items: center; justify-content: center; z-index: 1000; opacity: 0; pointer-events: none; transition: opacity 0.3s; }
        .modal.open { opacity: 1; pointer-events: auto; }
        .modal-card { background: #ffffff; border: 1px solid #e2d9c8; border-radius: 12px; padding: 40px; max-width: 450px; width: 90%; text-align: center; box-shadow: 0 20px 40px rgba(0,0,0,0.1); transform: scale(0.95); transition: transform 0.3s; }
        .modal.open .modal-card { transform: scale(1); }
        .modal-warning-icon { font-size: 3rem; margin-bottom: 15px; color: #c87a53; }
        .modal h2 { font-family: 'Playfair Display', serif; font-size: 1.8rem; margin-bottom: 10px; color: #2b1e16; }
        .modal p { color: #6b5c51; font-size: 0.95rem; margin-bottom: 30px; line-height: 1.5; }
        .modal-buttons { display: flex; gap: 15px; justify-content: center; }
        .btn-modal { padding: 12px 24px; border: none; border-radius: 8px; font-size: 0.95rem; font-weight: 500; font-family: 'Inter', sans-serif; cursor: pointer; transition: all 0.2s; }
        .btn-cancel { background: #f4eee4; color: #4a3e35; }
        .btn-cancel:hover { background: #e2d9c8; }
        .btn-confirm { background: #c87a53; color: #fff; }
        .btn-confirm:hover { background: #b06846; }
    </style>
</head>
<body>
    <header>
        <div class="header-title"><span>Academy Portal</span></div>
        <div class="student-badge">
            <div class="student-info">
                <div class="student-name"><!-- STUDENT_NAME --></div>
                <div class="student-roll">Roll No: <!-- STUDENT_ROLL --></div>
            </div>
        </div>
    </header>
    <div class="main-layout">
        <div class="sidebar">
            <div class="sidebar-card">
                <h3>Time Remaining</h3>
                <div class="timer-display" id="examTimer">10:00</div>
            </div>
            <div class="sidebar-card">
                <h3>Question Grid</h3>
                <div class="question-grid" id="navigatorGrid">
                    <!-- NAV_BUTTONS -->
                </div>
            </div>
        </div>
        <div class="content-area">
            <div class="exam-instructions">
                <strong>Instructions:</strong> Please answer all multiple choice questions. The timer is running. Once submitted, your score will be stored and shown instantly. Do not close or reload the browser.
            </div>
            <form id="examForm" action="/submit" method="POST">
                <!-- EXAM_QUESTIONS_PLACEHOLDER -->
                <div class="submit-section">
                     <button type="button" class="btn-submit-exam" onclick="openSubmitModal()">Submit Answer Paper</button>
                </div>
            </form>
        </div>
    </div>
    <div class="modal" id="submitModal">
        <div class="modal-card">
            <div class="modal-warning-icon">📜</div>
            <h2>Submit Your Exam?</h2>
            <p>You have answered <span id="answeredCount">0</span> of <span id="totalQuestionsCount">0</span> questions. You cannot change your answers after submission.</p>
            <div class="modal-buttons">
                <button class="btn-modal btn-cancel" onclick="closeSubmitModal()">Review Answers</button>
                <button class="btn-modal btn-confirm" onclick="confirmSubmit()">Submit Now</button>
            </div>
        </div>
    </div>
    <script>
        const totalQuestions = <!-- TOTAL_QUESTIONS_COUNT -->;
        let examTimeSeconds = <!-- EXAM_DURATION_SECONDS -->;
        function updateProgress() {
            let answered = 0;
            for (let i = 1; i <= totalQuestions; i++) {
                const radios = document.getElementsByName('q_' + i);
                let isAnswered = false;
                for (let r of radios) { if (r.checked) { isAnswered = true; break; } }
                const navBtn = document.getElementById('nav_btn_' + i);
                if (navBtn) { if (isAnswered) { navBtn.classList.add('answered'); answered++; } else { navBtn.classList.remove('answered'); } }
            }
            document.getElementById('answeredCount').innerText = answered;
            document.getElementById('totalQuestionsCount').innerText = totalQuestions;
        }
        function selectOption(radio) {
            const name = radio.name;
            document.getElementsByName(name).forEach(opt => {
                const label = opt.closest('.option-label');
                if (label) label.classList.remove('checked');
            });
            const label = radio.closest('.option-label');
            if (label) label.classList.add('checked');
            updateProgress();
        }
        document.querySelectorAll('.option-label input[type="radio"]').forEach(radio => {
            radio.addEventListener('change', () => selectOption(radio));
            if (radio.checked) selectOption(radio);
        });
        function scrollToQuestion(num) {
            const el = document.getElementById('question_card_' + num);
            if (el) {
                el.scrollIntoView({ behavior: 'smooth' });
                for (let i = 1; i <= totalQuestions; i++) {
                    const btn = document.getElementById('nav_btn_' + i);
                    if (btn) btn.classList.remove('current');
                }
                document.getElementById('nav_btn_' + num).classList.add('current');
            }
        }
        function openSubmitModal() { updateProgress(); document.getElementById('submitModal').classList.add('open'); }
        function closeSubmitModal() { document.getElementById('submitModal').classList.remove('open'); }
        function confirmSubmit() { document.getElementById('examForm').submit(); }
        const timerInterval = setInterval(() => {
            if (examTimeSeconds <= 0) {
                clearInterval(timerInterval);
                document.getElementById('examTimer').innerText = "00:00";
                document.getElementById('examTimer').style.color = "#c0392b";
                confirmSubmit(); return;
            }
            examTimeSeconds--;
            const minutes = Math.floor(examTimeSeconds / 60);
            const seconds = examTimeSeconds % 60;
            document.getElementById('examTimer').innerText = String(minutes).padStart(2, '0') + ":" + String(seconds).padStart(2, '0');
            if (examTimeSeconds < 60) { document.getElementById('examTimer').style.color = "#c0392b"; }
        }, 1000);
        updateProgress();
    </script>
</body>
</html>
)html";

const std::string RESULT_HTML_TEMPLATE = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Academy Portal - Results</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600&family=Playfair+Display:wght@600;700&display=swap" rel="stylesheet">
    <style>
        * { box-sizing: border-box; margin: 0; padding: 0; }
        body { background-color: #fdfbf7; color: #2b1e16; font-family: 'Inter', sans-serif; min-height: 100vh; display: flex; align-items: center; justify-content: center; padding: 40px 20px; }
        .container { width: 100%; max-width: 800px; display: flex; flex-direction: column; gap: 30px; }
        .result-card { background: #ffffff; border: 1px solid #e2d9c8; border-radius: 12px; padding: 40px; box-shadow: 0 10px 30px rgba(43, 30, 22, 0.05); text-align: center; }
        .result-header { margin-bottom: 30px; }
        h2 { font-family: 'Playfair Display', serif; font-weight: 700; font-size: 2rem; margin-bottom: 5px; color: #2b1e16; }
        .subtitle { color: #6b5c51; font-size: 0.95rem; }
        .score-visualization { display: flex; justify-content: center; align-items: center; margin: 25px 0; position: relative; }
        .circular-chart { width: 180px; height: 180px; }
        .circle-bg { fill: none; stroke: #f4eee4; stroke-width: 2.8; }
        .circle { fill: none; stroke-width: 2.8; stroke-linecap: round; transition: stroke-dasharray 1s ease-in-out; }
        .circle-pass { stroke: #8a9a86; }
        .circle-fail { stroke: #c87a53; }
        .percentage-text { position: absolute; font-family: 'Playfair Display', serif; font-size: 2.5rem; font-weight: 700; color: #2b1e16; }
        .pass-badge { display: inline-block; padding: 8px 24px; border-radius: 30px; font-weight: 600; font-size: 0.95rem; text-transform: uppercase; letter-spacing: 1px; margin-top: 10px; }
        .pass-badge.pass { background: #f4eee4; border: 1px solid #8a9a86; color: #758572; }
        .pass-badge.fail { background: #fdf3f2; border: 1px solid #e8b0b0; color: #c0392b; }
        .stats-grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 15px; margin: 30px 0; }
        .stat-box { background: #fdfbf7; border: 1px solid #e2d9c8; padding: 15px; border-radius: 8px; text-align: center; }
        .stat-value { font-family: 'Playfair Display', serif; font-size: 1.5rem; font-weight: 700; color: #2b1e16; margin-bottom: 5px; }
        .stat-label { font-size: 0.8rem; color: #8a9a86; text-transform: uppercase; letter-spacing: 0.5px; font-weight: 600; }
        .btn-action { display: inline-block; padding: 14px 35px; background: #c87a53; border: none; border-radius: 8px; color: #fff; font-size: 1rem; font-weight: 500; font-family: 'Inter', sans-serif; cursor: pointer; transition: all 0.3s; text-decoration: none; }
        .btn-action:hover { background: #b06846; transform: translateY(-2px); }
        .review-section { background: #ffffff; border: 1px solid #e2d9c8; border-radius: 12px; padding: 30px; box-shadow: 0 4px 15px rgba(43, 30, 22, 0.02); }
        .review-section h3 { font-family: 'Playfair Display', serif; font-size: 1.4rem; margin-bottom: 20px; font-weight: 700; color: #2b1e16; text-align: left; border-bottom: 1px solid #e2d9c8; padding-bottom: 10px; }
        .review-list { display: flex; flex-direction: column; gap: 20px; text-align: left; }
        .review-card { background: #fdfbf7; border: 1px solid #e2d9c8; border-radius: 8px; padding: 20px; }
        .review-card.correct { border-left: 4px solid #8a9a86; }
        .review-card.incorrect { border-left: 4px solid #c87a53; }
        .review-question { font-family: 'Playfair Display', serif; font-weight: 600; font-size: 1.1rem; color: #2b1e16; margin-bottom: 15px; }
        .review-answers { display: flex; flex-direction: column; gap: 8px; font-size: 0.9rem; }
        .answer-badge { display: flex; align-items: center; padding: 8px 12px; border-radius: 6px; width: fit-content; }
        .answer-badge.user-answer { background: #fdf3f2; color: #c0392b; border: 1px solid #e8b0b0; }
        .answer-badge.user-answer.correct-answer { background: #f4eee4; color: #758572; border: 1px solid #8a9a86; }
        .answer-badge.correct-sol { background: #f4eee4; color: #758572; border: 1px solid #8a9a86; margin-top: 4px; }
        .badge-label { font-weight: 600; margin-right: 8px; font-size: 0.8rem; text-transform: uppercase; letter-spacing: 0.5px; }
    </style>
</head>
<body>
    <div class="container">
        <div class="result-card">
            <div class="result-header">
                <h2>Exam Report Card</h2>
                <p class="subtitle">Roll No: <!-- STUDENT_ROLL --> &bull; <!-- STUDENT_NAME --></p>
            </div>
            <div class="score-visualization">
                <svg viewBox="0 0 36 36" class="circular-chart">
                    <path class="circle-bg" d="M18 2.0845 a 15.9155 15.9155 0 0 1 0 31.831 a 15.9155 15.9155 0 0 1 0 -31.831" />
                    <path class="circle <!-- PERCENT_CLASS -->" stroke-dasharray="<!-- PERCENT_VALUE -->, 100" d="M18 2.0845 a 15.9155 15.9155 0 0 1 0 31.831 a 15.9155 15.9155 0 0 1 0 -31.831" />
                </svg>
                <div class="percentage-text"><!-- PERCENT_TEXT -->%</div>
            </div>
            <div class="pass-badge <!-- PASS_CLASS -->"><!-- PASS_TEXT --></div>
            <div class="stats-grid">
                <div class="stat-box">
                    <div class="stat-value"><!-- SCORE_OBTAINED --> / <!-- TOTAL_MARKS --></div>
                    <div class="stat-label">Marks Gained</div>
                </div>
                <div class="stat-box">
                    <div class="stat-value"><!-- CORRECT_COUNT --></div>
                    <div class="stat-label">Correct Answers</div>
                </div>
                <div class="stat-box">
                    <div class="stat-value"><!-- INCORRECT_COUNT --></div>
                    <div class="stat-label">Wrong Answers</div>
                </div>
            </div>
            <a href="/logout" class="btn-action">Log Out & Exit</a>
        </div>
        <div class="review-section">
            <h3>Question Performance Review</h3>
            <div class="review-list">
                <!-- PERFORMANCE_REVIEW_ITEMS -->
            </div>
        </div>
    </div>
</body>
</html>
)html";

// Helper function to read a template from disk or fallback
std::string loadTemplate(const std::string& filename, const std::string& fallback) {
    std::ifstream file("templates/" + filename);
    if (!file.is_open()) return fallback;
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

// Reusable String Replacement Utility
void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

// Reusable HTML Escaping Utility to prevent Cross-Site Scripting (XSS)
std::string escapeHTML(const std::string& data) {
    std::string buffer;
    buffer.reserve(data.size());
    for (size_t pos = 0; pos != data.size(); ++pos) {
        switch (data[pos]) {
            case '&':  buffer.append("&amp;");       break;
            case '\"': buffer.append("&quot;");      break;
            case '\'': buffer.append("&apos;");      break;
            case '<':  buffer.append("&lt;");        break;
            case '>':  buffer.append("&gt;");        break;
            default:   buffer.append(&data[pos], 1); break;
        }
    }
    return buffer;
}

void createTemplatesIfNeeded() {
#ifdef _WIN32
    CreateDirectoryA("templates", NULL);
#else
    system("mkdir -p templates");
#endif

    auto writeTemplate = [](const std::string& path, const std::string& content) {
        std::ofstream out(path, std::ios::trunc);
        if (out.is_open()) {
            out << content;
        }
    };

    writeTemplate("templates/login.html", LOGIN_HTML_TEMPLATE);
    writeTemplate("templates/exam.html", EXAM_HTML_TEMPLATE);
    writeTemplate("templates/result.html", RESULT_HTML_TEMPLATE);
    writeTemplate("templates/already_submitted.html", ALREADY_SUBMITTED_HTML_TEMPLATE);
}

// ============================================================================
// WEB ADMIN PANEL — AUTHENTICATION & PAGE BUILDERS
// ============================================================================
bool isAdminAuthenticated(const std::map<std::string, std::string>& headers) {
    auto it = headers.find("cookie");
    if (it == headers.end()) it = headers.find("Cookie");
    if (it == headers.end()) return false;
    return it->second.find(ADMIN_COOKIE_NAME + "=" + ADMIN_SESSION_TOKEN) != std::string::npos;
}

std::string buildAdminLayout(const std::string& pageTitle,
                              const std::string& content,
                              const std::string& activeTab) {
    auto navLink = [&](const std::string& href, const std::string& icon,
                       const std::string& label, const std::string& tab) {
        std::string cls = (tab == activeTab) ? " active" : "";
        return "<a href=\"" + href + "\" class=\"nav-link" + cls + "\"><span class=\"nav-icon\">"
               + icon + "</span>" + label + "</a>\n";
    };
    return "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n"
           "<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1.0\">\n"
           "<title>" + pageTitle + " — Admin Panel</title>\n"
           "<link href=\"https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600&family=Playfair+Display:wght@600;700&display=swap\" rel=\"stylesheet\">\n"
           "<style>\n"
           "* { box-sizing:border-box; margin:0; padding:0; }\n"
           "body { display:flex; min-height:100vh; background:#fdfbf7; color:#2b1e16; font-family:'Inter',sans-serif; }\n"
           ".sidebar { width:260px; background:#ffffff; border-right:1px solid #e2d9c8;\n"
           "  display:flex; flex-direction:column; padding:30px 20px;\n"
           "  position:fixed; height:100vh; }\n"
           ".logo { display:flex; align-items:center; gap:12px; margin-bottom:35px; padding:0 4px; }\n"
           ".logo-icon { width:36px; height:36px; background:#f4eee4; border-radius:8px;\n"
           "  display:flex; align-items:center; justify-content:center; color:#8a9a86;\n"
           "  font-size:20px; border:1px solid #d4c5b0; }\n"
           ".logo-text { font-family:'Playfair Display',serif; font-size:1.15rem; font-weight:700; color:#2b1e16; line-height:1.2; }\n"
           ".logo-sub { font-size:0.75rem; color:#6b5c51; font-weight:500; }\n"
           ".nav-section { font-size:0.7rem; font-weight:600; color:#8a9a86; text-transform:uppercase;\n"
           "  letter-spacing:1px; padding:0 8px; margin-bottom:12px; }\n"
           ".nav-link { display:flex; align-items:center; gap:12px; padding:12px 14px;\n"
           "  border-radius:8px; margin-bottom:6px; color:#6b5c51; font-size:0.9rem;\n"
           "  font-weight:500; text-decoration:none; transition:all .2s; border:1px solid transparent; }\n"
           ".nav-link:hover { background:#fdfbf7; color:#2b1e16; border-color:#f4eee4; }\n"
           ".nav-link.active { background:#f4eee4; color:#758572; font-weight:600; border-color:#e2d9c8; }\n"
           ".nav-icon { font-size:16px; width:20px; text-align:center; display:inline-flex; align-items:center; justify-content:center; color:#6b5c51; }\n"
           ".nav-link.active .nav-icon { color:#758572; }\n"
           ".nav-spacer { flex:1; }\n"
           ".logout-link { display:flex; align-items:center; gap:12px; padding:12px 14px;\n"
           "  border-radius:8px; color:#6b5c51; font-size:0.9rem; text-decoration:none; transition:all .2s; border:1px solid transparent; }\n"
           ".logout-link:hover { background:#fdf3f2; color:#c0392b; border-color:#e8b0b0; }\n"
           ".main-area { margin-left:260px; flex:1; display:flex; flex-direction:column; min-width:0; }\n"
           ".topbar { background:#ffffff; border-bottom:1px solid #e2d9c8;\n"
           "  padding:20px 40px; display:flex; align-items:center; justify-content:space-between;\n"
           "  position:sticky; top:0; z-index:50; }\n"
           ".page-title { font-family:'Playfair Display',serif; font-size:1.5rem; font-weight:700; color:#2b1e16; }\n"
           ".admin-chip { background:#f4eee4; border:1px solid #d4c5b0;\n"
           "  border-radius:8px; padding:6px 14px; font-size:0.75rem; font-weight:600; color:#6b5c51; display:flex; align-items:center; gap:6px; }\n"
           ".content { padding:40px; flex:1; }\n"
           ".stat-grid { display:grid; grid-template-columns:repeat(auto-fit,minmax(220px,1fr)); gap:24px; margin-bottom:40px; }\n"
           ".stat-card { background:#ffffff; border:1px solid #e2d9c8;\n"
           "  border-radius:12px; padding:28px; box-shadow:0 4px 15px rgba(43, 30, 22, 0.02); display:flex; flex-direction:column; justify-content:space-between; position:relative; overflow:hidden; }\n"
           ".stat-card::before { content:''; position:absolute; top:0; left:0; width:4px; height:100%; }\n"
           ".stat-card.orange::before { background:#c87a53; }\n"
           ".stat-card.blue::before { background:#8a9a86; }\n"
           ".stat-card.green::before { background:#758572; }\n"
           ".stat-card.purple::before { background:#b06846; }\n"
           ".stat-n { font-family:'Playfair Display',serif; font-size:2.2rem; font-weight:700; color:#2b1e16; }\n"
           ".stat-l { font-size:0.85rem; color:#6b5c51; margin-top:6px; font-weight:500; text-transform:uppercase; letter-spacing:0.5px; }\n"
           ".card { background:#ffffff; border:1px solid #e2d9c8;\n"
           "  border-radius:12px; padding:32px; margin-bottom:30px; box-shadow:0 4px 15px rgba(43, 30, 22, 0.02); }\n"
           ".card h3 { font-family:'Playfair Display',serif; font-size:1.2rem; font-weight:700; color:#2b1e16; margin-bottom:24px;\n"
           "  padding-bottom:12px; border-bottom:1px solid #f4eee4; }\n"
           "table { width:100%; border-collapse:collapse; }\n"
           "th { font-size:0.75rem; font-weight:600; color:#8a9a86; text-transform:uppercase;\n"
           "  letter-spacing:.5px; padding:12px 16px; text-align:left;\n"
           "  border-bottom:1px solid #e2d9c8; }\n"
           "td { padding:16px; font-size:0.9rem; color:#2b1e16;\n"
           "  border-bottom:1px solid #f4eee4; }\n"
           "tr:hover td { background:#fdfbf7; }\n"
           ".fg { display:grid; grid-template-columns:repeat(auto-fit,minmax(220px,1fr)); gap:20px; margin-bottom:24px; }\n"
           ".fg label { display:block; font-size:0.75rem; font-weight:600; color:#6b5c51;\n"
           "  text-transform:uppercase; letter-spacing:.5px; margin-bottom:8px; }\n"
           ".fg input, .fg select { width:100%; padding:12px 16px;\n"
           "  background:#ffffff; border:1px solid #d4c5b0;\n"
           "  border-radius:8px; color:#2b1e16; font-size:0.9rem; transition:all .2s; font-family:inherit; }\n"
           ".fg input:focus,.fg select:focus { outline:none; border-color:#c87a53;\n"
           "  box-shadow:0 0 0 3px rgba(200,122,83,0.1); }\n"
           ".fg select option { background:#ffffff; color:#2b1e16; }\n"
           ".btn { padding:12px 24px; border:none; border-radius:8px; font-size:0.9rem;\n"
           "  font-weight:600; cursor:pointer; transition:all .2s; display:inline-flex; align-items:center; justify-content:center; font-family:inherit; }\n"
           ".btn-add { background:#c87a53; color:#fff;\n"
           "  box-shadow:0 4px 12px rgba(200, 122, 83, 0.2); }\n"
           ".btn-add:hover { background:#b06846; transform:translateY(-1px); }\n"
           ".btn-del { background:#fdf3f2; color:#c0392b;\n"
           "  border:1px solid #e8b0b0; padding:8px 16px; font-size:0.8rem; border-radius:6px; }\n"
           ".btn-del:hover { background:#fcdddb; }\n"
           ".badge-pass { background:#f4eee4; color:#758572;\n"
           "  border:1px solid #8a9a86; padding:4px 12px;\n"
           "  border-radius:20px; font-size:0.75rem; font-weight:600; text-transform:uppercase; letter-spacing:0.5px; }\n"
           ".badge-fail { background:#fdf3f2; color:#c0392b;\n"
           "  border:1px solid #e8b0b0; padding:4px 12px;\n"
           "  border-radius:20px; font-size:0.75rem; font-weight:600; text-transform:uppercase; letter-spacing:0.5px; }\n"
           ".msg { border-radius:8px; padding:14px 20px; font-size:0.9rem; margin-bottom:24px; display:flex; align-items:center; gap:8px; }\n"
           ".msg-ok { background:#f4eee4; border:1px solid #8a9a86; color:#758572; }\n"
           ".msg-err { background:#fdf3f2; border:1px solid #e8b0b0; color:#c0392b; }\n"
           ".empty { text-align:center; padding:48px; color:#6b5c51; font-size:0.9rem; font-style:italic; }\n"
           "</style>\n</head>\n<body>\n"
           "<div class=\"sidebar\">\n"
           "  <div class=\"logo\"><div class=\"logo-icon\">🎓</div>\n"
           "    <div><div class=\"logo-text\">Academy</div><div class=\"logo-sub\">Admin Portal</div></div></div>\n"
           "  <div class=\"nav-section\">Management</div>\n"
           + navLink("/admin/dashboard", "<svg viewBox=\"0 0 24 24\" width=\"16\" height=\"16\" stroke=\"currentColor\" stroke-width=\"2\" fill=\"none\" stroke-linecap=\"round\" stroke-linejoin=\"round\"><rect x=\"3\" y=\"3\" width=\"7\" height=\"9\"></rect><rect x=\"14\" y=\"3\" width=\"7\" height=\"5\"></rect><rect x=\"14\" y=\"12\" width=\"7\" height=\"9\"></rect><rect x=\"3\" y=\"16\" width=\"7\" height=\"5\"></rect></svg>", "Dashboard", "dashboard")
           + navLink("/admin/students",  "<svg viewBox=\"0 0 24 24\" width=\"16\" height=\"16\" stroke=\"currentColor\" stroke-width=\"2\" fill=\"none\" stroke-linecap=\"round\" stroke-linejoin=\"round\"><path d=\"M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2\"></path><circle cx=\"9\" cy=\"7\" r=\"4\"></circle><path d=\"M23 21v-2a4 4 0 0 0-3-3.87\"></path><path d=\"M16 3.13a4 4 0 0 1 0 7.75\"></path></svg>", "Students", "students")
           + navLink("/admin/questions", "<svg viewBox=\"0 0 24 24\" width=\"16\" height=\"16\" stroke=\"currentColor\" stroke-width=\"2\" fill=\"none\" stroke-linecap=\"round\" stroke-linejoin=\"round\"><path d=\"M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z\"></path><polyline points=\"14 2 14 8 20 8\"></polyline><line x1=\"16\" y1=\"13\" x2=\"8\" y2=\"13\"></line><line x1=\"16\" y1=\"17\" x2=\"8\" y2=\"17\"></line><polyline points=\"10 9 9 9 8 9\"></polyline></svg>", "Questions", "questions")
           + navLink("/admin/results",   "<svg viewBox=\"0 0 24 24\" width=\"16\" height=\"16\" stroke=\"currentColor\" stroke-width=\"2\" fill=\"none\" stroke-linecap=\"round\" stroke-linejoin=\"round\"><line x1=\"18\" y1=\"20\" x2=\"18\" y2=\"10\"></line><line x1=\"12\" y1=\"20\" x2=\"12\" y2=\"4\"></line><line x1=\"6\" y1=\"20\" x2=\"6\" y2=\"14\"></line></svg>", "Results", "results")
           + "  <div class=\"nav-spacer\"></div>\n"
           "  <a href=\"/admin/logout\" class=\"logout-link\">"
           "<span class=\"nav-icon\"><svg viewBox=\"0 0 24 24\" width=\"16\" height=\"16\" stroke=\"currentColor\" stroke-width=\"2\" fill=\"none\" stroke-linecap=\"round\" stroke-linejoin=\"round\"><path d=\"M9 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h4\"></path><polyline points=\"16 17 21 12 16 7\"></polyline><line x1=\"21\" y1=\"12\" x2=\"9\" y2=\"12\"></line></svg></span>"
           "Logout</a>\n"
           "</div>\n"
           "<div class=\"main-area\">\n"
           "  <div class=\"topbar\">\n"
           "    <div class=\"page-title\">" + pageTitle + "</div>\n"
           "    <div class=\"admin-chip\">🔒 Admin</div>\n"
           "  </div>\n"
           "  <div class=\"content\">" + content + "</div>\n"
           "</div>\n</body>\n</html>";
}

std::string getAdminDashboardHTML(DBManager& db) {
    auto students  = db.getStudentsCopy();
    auto questions = db.getQuestionsCopy();
    auto results   = db.getResultsCopy();
    int passCount = 0;
    for (const auto& r : results) {
        int pct = r.totalMarks > 0 ? (r.score * 100 / r.totalMarks) : 0;
        if (pct >= 40) passCount++;
    }
    int passRate = results.empty() ? 0 : (passCount * 100 / (int)results.size());

    std::string stats =
        "<div class=\"stat-grid\">"
        "<div class=\"stat-card orange\"><div class=\"stat-n\">" + std::to_string(students.size()) + "</div><div class=\"stat-l\">Total Students</div></div>"
        "<div class=\"stat-card blue\"><div class=\"stat-n\">" + std::to_string(questions.size()) + "</div><div class=\"stat-l\">Questions in Paper</div></div>"
        "<div class=\"stat-card green\"><div class=\"stat-n\">" + std::to_string(results.size()) + "</div><div class=\"stat-l\">Submissions</div></div>"
        "<div class=\"stat-card purple\"><div class=\"stat-n\">" + std::to_string(passRate) + "%</div><div class=\"stat-l\">Pass Rate</div></div>"
        "</div>";

    std::string recent = "<div class=\"card\"><h3>Recent Submissions</h3>";
    if (results.empty()) {
        recent += "<div class=\"empty\">No submissions yet.</div>";
    } else {
        recent += "<table><tr><th>Roll No</th><th>Name</th><th>Score</th><th>%</th><th>Status</th><th>Time</th></tr>";
        for (int i = (int)results.size()-1; i >= 0; --i) {
            const auto& r = results[i];
            int pct = r.totalMarks > 0 ? (r.score * 100 / r.totalMarks) : 0;
            bool pass = pct >= 40;
            recent += "<tr><td>" + escapeHTML(r.rollNumber) + "</td><td>" + escapeHTML(r.studentName)
                    + "</td><td>" + std::to_string(r.score) + "/" + std::to_string(r.totalMarks)
                    + "</td><td>" + std::to_string(pct) + "%</td>"
                    + "<td><span class=\"" + std::string(pass ? "badge-pass" : "badge-fail") + "\">"
                    + (pass ? "Pass" : "Fail") + "</span></td>"
                    + "<td>" + escapeHTML(r.timestamp) + "</td></tr>";
        }
        recent += "</table>";
    }
    recent += "</div>";

    return buildAdminLayout("Dashboard", stats + recent, "dashboard");
}

std::string getAdminStudentsHTML(DBManager& db, const std::string& msg, bool isError) {
    std::string msgHtml;
    if (!msg.empty()) msgHtml = "<div class=\"msg " + std::string(isError ? "msg-err" : "msg-ok") + "\">" + msg + "</div>";

    std::string form =
        "<div class=\"card\"><h3>Add New Student</h3>" + msgHtml +
        "<form method=\"POST\" action=\"/admin/students/add\">"
        "<div class=\"fg\">"
        "<div><label>Roll Number / ID</label><input type=\"text\" name=\"roll\" placeholder=\"e.g. 104\" required></div>"
        "<div><label>Full Name</label><input type=\"text\" name=\"name\" placeholder=\"Student Name\" required></div>"
        "<div><label>Password</label><input type=\"text\" name=\"password\" placeholder=\"Login password\" required></div>"
        "</div>"
        "<button class=\"btn btn-add\" type=\"submit\">＋ Add Student</button>"
        "</form></div>";

    auto students = db.getStudentsCopy();
    auto results  = db.getResultsCopy();
    std::string table = "<div class=\"card\"><h3>All Students (" + std::to_string(students.size()) + ")</h3>";
    if (students.empty()) {
        table += "<div class=\"empty\">No students registered yet. Add one above.</div>";
    } else {
        table += "<table><tr><th>Roll No</th><th>Name</th><th>Password</th><th>Submitted</th><th>Action</th></tr>";
        for (const auto& s : students) {
            bool submitted = false;
            for (const auto& r : results) if (r.rollNumber == s.rollNumber) { submitted = true; break; }
            table += "<tr><td>" + escapeHTML(s.rollNumber) + "</td><td>" + escapeHTML(s.name) + "</td><td>" + escapeHTML(s.password) + "</td>"
                   + "<td>" + (submitted ? "<span class=\"badge-pass\">Yes</span>" : "<span style=\"color:#475569\">No</span>") + "</td>"
                   + "<td><form method=\"POST\" action=\"/admin/students/delete\" style=\"display:inline\">"
                   + "<input type=\"hidden\" name=\"roll\" value=\"" + escapeHTML(s.rollNumber) + "\">"
                   + "<button class=\"btn btn-del\" type=\"submit\" onclick=\"return confirm('Delete " + escapeHTML(s.name) + "?')\">Delete</button>"
                   + "</form></td></tr>";
        }
        table += "</table>";
    }
    table += "</div>";

    return buildAdminLayout("Student Management", form + table, "students");
}

std::string getAdminQuestionsHTML(DBManager& db, const std::string& msg, bool isError) {
    std::string msgHtml;
    if (!msg.empty()) msgHtml = "<div class=\"msg " + std::string(isError ? "msg-err" : "msg-ok") + "\">" + msg + "</div>";

    std::string form =
        "<div class=\"card\"><h3>Add Question</h3>" + msgHtml +
        "<form method=\"POST\" action=\"/admin/questions/add\">"
        "<div class=\"fg\" style=\"grid-template-columns:1fr\">"
        "<div><label>Question Text</label><input type=\"text\" name=\"text\" placeholder=\"Enter question...\" required></div>"
        "</div>"
        "<div class=\"fg\">"
        "<div><label>Option A</label><input type=\"text\" name=\"opta\" placeholder=\"Option A\" required></div>"
        "<div><label>Option B</label><input type=\"text\" name=\"optb\" placeholder=\"Option B\" required></div>"
        "<div><label>Option C</label><input type=\"text\" name=\"optc\" placeholder=\"Option C\" required></div>"
        "<div><label>Option D</label><input type=\"text\" name=\"optd\" placeholder=\"Option D\" required></div>"
        "<div><label>Correct Answer</label>"
        "<select name=\"correct\"><option value=\"A\">A</option><option value=\"B\">B</option>"
        "<option value=\"C\">C</option><option value=\"D\">D</option></select></div>"
        "<div><label>Marks</label><input type=\"number\" name=\"marks\" value=\"2\" min=\"1\" required></div>"
        "</div>"
        "<button class=\"btn btn-add\" type=\"submit\">＋ Add Question</button>"
        "</form></div>";

    auto questions = db.getQuestionsCopy();
    int total = 0; for (const auto& q : questions) total += q.marks;
    std::string table = "<div class=\"card\"><h3>Question Paper (" + std::to_string(questions.size())
                      + " questions, " + std::to_string(total) + " total marks)</h3>";
    if (questions.empty()) {
        table += "<div class=\"empty\">No questions added yet. Add one above.</div>";
    } else {
        table += "<table><tr><th>ID</th><th>Question</th><th>Options</th><th>Answer</th><th>Marks</th><th>Action</th></tr>";
        for (const auto& q : questions) {
            table += "<tr><td>" + std::to_string(q.id) + "</td><td>" + escapeHTML(q.text) + "</td>"
                   + "<td style=\"font-size:12px;color:#64748b\">A:" + escapeHTML(q.optionA) + " B:" + escapeHTML(q.optionB)
                   + "<br>C:" + escapeHTML(q.optionC) + " D:" + escapeHTML(q.optionD) + "</td>"
                   + "<td><strong>" + std::string(1, q.correctOption) + "</strong></td>"
                   + "<td>" + std::to_string(q.marks) + "</td>"
                   + "<td><form method=\"POST\" action=\"/admin/questions/delete\" style=\"display:inline\">"
                   + "<input type=\"hidden\" name=\"id\" value=\"" + std::to_string(q.id) + "\">"
                   + "<button class=\"btn btn-del\" type=\"submit\" onclick=\"return confirm('Delete this question?')\">Delete</button>"
                   + "</form></td></tr>";
        }
        table += "</table>";
    }
    table += "</div>";

    return buildAdminLayout("Question Management", form + table, "questions");
}

std::string getAdminResultsHTML(DBManager& db) {
    auto results = db.getResultsCopy();
    std::string content = "<div class=\"card\"><h3>All Exam Results (" + std::to_string(results.size()) + " submissions)</h3>";
    if (results.empty()) {
        content += "<div class=\"empty\">No students have submitted the exam yet.</div>";
    } else {
        content += "<table><tr><th>Roll No</th><th>Name</th><th>Score</th><th>Total</th><th>%</th><th>Status</th><th>Submitted At</th></tr>";
        for (const auto& r : results) {
            int pct = r.totalMarks > 0 ? (r.score * 100 / r.totalMarks) : 0;
            bool pass = pct >= 40;
            content += "<tr><td>" + escapeHTML(r.rollNumber) + "</td><td>" + escapeHTML(r.studentName)
                     + "</td><td>" + std::to_string(r.score) + "</td><td>" + std::to_string(r.totalMarks)
                     + "</td><td>" + std::to_string(pct) + "%</td>"
                     + "<td><span class=\"" + std::string(pass ? "badge-pass" : "badge-fail") + "\">"
                     + (pass ? "✓ Pass" : "✗ Fail") + "</span></td>"
                     + "<td>" + escapeHTML(r.timestamp) + "</td></tr>";
        }
        content += "</table>";
    }
    content += "</div>";
    return buildAdminLayout("Exam Results", content, "results");
}
