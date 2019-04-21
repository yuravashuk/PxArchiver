namespace PxArchiverApp
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.FilesList = new System.Windows.Forms.ListBox();
            this.AddButton = new System.Windows.Forms.Button();
            this.RemoveButton = new System.Windows.Forms.Button();
            this.BuildButton = new System.Windows.Forms.Button();
            this.ExportButton = new System.Windows.Forms.Button();
            this.AddFolder = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.languageTextbox = new System.Windows.Forms.TextBox();
            this.typeTextbox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.compressedTextbox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.saveOptionsButton = new System.Windows.Forms.Button();
            this.editMetadataButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(28, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Files";
            // 
            // FilesList
            // 
            this.FilesList.FormattingEnabled = true;
            this.FilesList.Location = new System.Drawing.Point(16, 30);
            this.FilesList.Name = "FilesList";
            this.FilesList.Size = new System.Drawing.Size(394, 303);
            this.FilesList.TabIndex = 1;
            this.FilesList.SelectedIndexChanged += new System.EventHandler(this.FilesList_SelectedIndexChanged);
            // 
            // AddButton
            // 
            this.AddButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.AddButton.Location = new System.Drawing.Point(416, 209);
            this.AddButton.Name = "AddButton";
            this.AddButton.Size = new System.Drawing.Size(74, 36);
            this.AddButton.TabIndex = 2;
            this.AddButton.Text = "Add File";
            this.AddButton.UseVisualStyleBackColor = true;
            this.AddButton.Click += new System.EventHandler(this.AddButton_Click);
            // 
            // RemoveButton
            // 
            this.RemoveButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.RemoveButton.Location = new System.Drawing.Point(506, 209);
            this.RemoveButton.Name = "RemoveButton";
            this.RemoveButton.Size = new System.Drawing.Size(74, 36);
            this.RemoveButton.TabIndex = 3;
            this.RemoveButton.Text = "Remove File";
            this.RemoveButton.UseVisualStyleBackColor = true;
            this.RemoveButton.Click += new System.EventHandler(this.RemoveButton_Click);
            // 
            // BuildButton
            // 
            this.BuildButton.BackColor = System.Drawing.Color.DarkRed;
            this.BuildButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.BuildButton.ForeColor = System.Drawing.Color.White;
            this.BuildButton.Location = new System.Drawing.Point(506, 297);
            this.BuildButton.Name = "BuildButton";
            this.BuildButton.Size = new System.Drawing.Size(74, 36);
            this.BuildButton.TabIndex = 4;
            this.BuildButton.Text = "Build";
            this.BuildButton.UseVisualStyleBackColor = false;
            this.BuildButton.Click += new System.EventHandler(this.BuildButton_Click);
            // 
            // ExportButton
            // 
            this.ExportButton.BackColor = System.Drawing.Color.DarkGreen;
            this.ExportButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ExportButton.ForeColor = System.Drawing.Color.White;
            this.ExportButton.Location = new System.Drawing.Point(416, 297);
            this.ExportButton.Name = "ExportButton";
            this.ExportButton.Size = new System.Drawing.Size(74, 36);
            this.ExportButton.TabIndex = 5;
            this.ExportButton.Text = "Export";
            this.ExportButton.UseVisualStyleBackColor = false;
            this.ExportButton.Click += new System.EventHandler(this.ExportButton_Click);
            // 
            // AddFolder
            // 
            this.AddFolder.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.AddFolder.Location = new System.Drawing.Point(416, 251);
            this.AddFolder.Name = "AddFolder";
            this.AddFolder.Size = new System.Drawing.Size(74, 36);
            this.AddFolder.TabIndex = 6;
            this.AddFolder.Text = "Add Folder";
            this.AddFolder.UseVisualStyleBackColor = true;
            this.AddFolder.Click += new System.EventHandler(this.AddFolder_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(413, 30);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(55, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "Language";
            // 
            // languageTextbox
            // 
            this.languageTextbox.Location = new System.Drawing.Point(416, 47);
            this.languageTextbox.Name = "languageTextbox";
            this.languageTextbox.Size = new System.Drawing.Size(164, 20);
            this.languageTextbox.TabIndex = 8;
            // 
            // typeTextbox
            // 
            this.typeTextbox.Location = new System.Drawing.Point(416, 95);
            this.typeTextbox.Name = "typeTextbox";
            this.typeTextbox.Size = new System.Drawing.Size(164, 20);
            this.typeTextbox.TabIndex = 10;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(413, 78);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(31, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Type";
            // 
            // compressedTextbox
            // 
            this.compressedTextbox.Location = new System.Drawing.Point(416, 141);
            this.compressedTextbox.Name = "compressedTextbox";
            this.compressedTextbox.Size = new System.Drawing.Size(164, 20);
            this.compressedTextbox.TabIndex = 12;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(413, 124);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(65, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "Compressed";
            // 
            // saveOptionsButton
            // 
            this.saveOptionsButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.saveOptionsButton.Location = new System.Drawing.Point(416, 167);
            this.saveOptionsButton.Name = "saveOptionsButton";
            this.saveOptionsButton.Size = new System.Drawing.Size(74, 36);
            this.saveOptionsButton.TabIndex = 13;
            this.saveOptionsButton.Text = "Save Options";
            this.saveOptionsButton.UseVisualStyleBackColor = true;
            this.saveOptionsButton.Click += new System.EventHandler(this.saveOptionsButton_Click);
            // 
            // editMetadataButton
            // 
            this.editMetadataButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.editMetadataButton.Location = new System.Drawing.Point(506, 167);
            this.editMetadataButton.Name = "editMetadataButton";
            this.editMetadataButton.Size = new System.Drawing.Size(74, 36);
            this.editMetadataButton.TabIndex = 14;
            this.editMetadataButton.Text = "Edit Meta";
            this.editMetadataButton.UseVisualStyleBackColor = true;
            this.editMetadataButton.Click += new System.EventHandler(this.editMetadataButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(595, 342);
            this.Controls.Add(this.editMetadataButton);
            this.Controls.Add(this.saveOptionsButton);
            this.Controls.Add(this.compressedTextbox);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.typeTextbox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.languageTextbox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.AddFolder);
            this.Controls.Add(this.ExportButton);
            this.Controls.Add(this.BuildButton);
            this.Controls.Add(this.RemoveButton);
            this.Controls.Add(this.AddButton);
            this.Controls.Add(this.FilesList);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Archiver App";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListBox FilesList;
        private System.Windows.Forms.Button AddButton;
        private System.Windows.Forms.Button RemoveButton;
        private System.Windows.Forms.Button BuildButton;
        private System.Windows.Forms.Button ExportButton;
        private System.Windows.Forms.Button AddFolder;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox languageTextbox;
        private System.Windows.Forms.TextBox typeTextbox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox compressedTextbox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button saveOptionsButton;
        private System.Windows.Forms.Button editMetadataButton;
    }
}

