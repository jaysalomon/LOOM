#!/bin/bash
# LOOM Documentation Cleanup Script
# Removes redundant documentation files, keeping .tex as master

echo "LOOM Documentation Cleanup"
echo "=========================="
echo ""
echo "This will remove redundant documentation files."
echo "The master documentation will remain in loom_complete_documentation.tex"
echo ""
echo "Files to be removed:"
echo "- loom_complete_framework.md (redundant with .tex)"
echo "- loom-master-spec.md (redundant with .tex)"
echo "- loom_register_architecture.md (covered in .tex)"
echo "- loom_mathematics.md (covered in .tex)"
echo "- README_NEW.md (redundant with README.md)"
echo "- FULL_PROJECT_PLAN.md (merge with PROJECT_STRUCTURE.md)"
echo "- Kernelidea.md (integrate key ideas into wiki)"
echo ""
echo "Files to KEEP:"
echo "- README.md (main project readme)"
echo "- CONTRIBUTING.md (contribution guidelines)"
echo "- HONEST_STATUS.md (transparency about project state)"
echo "- PROJECT_STRUCTURE.md (directory organization)"
echo "- BUILD_AND_RUN.md (practical instructions)"
echo "- loom_complete_documentation.tex (MASTER DOCUMENT)"
echo ""
read -p "Proceed with cleanup? (y/n): " confirm

if [ "$confirm" = "y" ]; then
    # Create backup directory
    mkdir -p .deprecated_docs

    # Move redundant files to backup
    echo "Moving files to .deprecated_docs/ for backup..."

    mv loom_complete_framework.md .deprecated_docs/ 2>/dev/null
    mv loom-master-spec.md .deprecated_docs/ 2>/dev/null
    mv loom_register_architecture.md .deprecated_docs/ 2>/dev/null
    mv loom_mathematics.md .deprecated_docs/ 2>/dev/null
    mv README_NEW.md .deprecated_docs/ 2>/dev/null
    mv FULL_PROJECT_PLAN.md .deprecated_docs/ 2>/dev/null
    mv Kernelidea.md .deprecated_docs/ 2>/dev/null

    echo "✓ Files moved to .deprecated_docs/ (not deleted, just archived)"
    echo ""
    echo "Next steps:"
    echo "1. Review .deprecated_docs/ for any unique content to preserve"
    echo "2. Extract unique content and add to wiki or .tex file"
    echo "3. Once confident, you can delete .deprecated_docs/"
    echo "4. Set up GitHub wiki with the structure provided"
else
    echo "Cleanup cancelled."
fi