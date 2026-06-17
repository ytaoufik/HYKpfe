INSERT OR IGNORE INTO users(email, display_name, role, created_at)
VALUES
('teacher@hyk.local', 'Teacher Demo', 'teacher', datetime('now')),
('student1@hyk.local', 'Lina', 'student', datetime('now')),
('student2@hyk.local', 'Youssef', 'student', datetime('now'));
